#include "CPL_MGAttackFireRange.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_MouseController.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Particle/CParticle_FireRain.h"

//Widget
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_SkillCastingBar.h"

UCPL_MGAttackFireRange::UCPL_MGAttackFireRange()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	FString Path = L"";

	#pragma region MageAttackMontage
	// MageAttackMontage
	{
		UAnimMontage* Mage_FireRange= nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_Skilling_FireRange.Mage_Skilling_FireRange'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFireRange(*Path);
		if (MFireRange.Succeeded())
			Mage_FireRange = MFireRange.Object;

		MageAttackMontages.Emplace(Mage_FireRange);
	}
		
	#pragma endregion

	#pragma region DamageType

	DT_Normal = NewObject<UCDamageType_Normal>();
	DT_Normal->SetDamageImpulse(10.0f);

	DT_Burn = NewObject<UCDamageType_Burn>();
	DT_Burn->SetDamageImpulse(0.0f);
	DT_Burn->SetSecondDamageValue(3.0f);
	DT_Burn->SetBurnTime(5.0f);

	#pragma endregion
}

void UCPL_MGAttackFireRange::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@UNABLE - Auto AttackDecision System 
	AttackDecision->UnAble();

	#pragma endregion

	#pragma region Spawn Particle Object

	FTransform Transform = FTransform::Identity;
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	FireRainActor = GetWorld()->SpawnActor<ACParticle_FireRain>(ACParticle_FireRain::StaticClass(), Transform, Params);
	FireRainActor->OffEndActor();
	FireRainActor->SetDamageType(DT_Burn, DT_Normal);

	#pragma endregion

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		SkillCastWidget->EndProgress();
		bFillingReadyGauge = false;
		bFillOutGauge = false;

		//@StopMontage
		Player->ActorStopAnimMon(MageAttackMontages[0]);
	});

	// @EndAttack Delegate
	EndAttackDeleFunc.AddLambda([&]()
	{
		SkillCastWidget->EndProgress();
		bMouseWaiting = false;
		bFillingReadyGauge = false;
		bFillOutGauge = false;

		//@StopMontage
		Player->ActorStopAnimMon(MageAttackMontages[0]);

		//@Off Particle Object
		FireRainActor->OffEndActor();

		Player->OffBlockAction();
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	//EndAttackDeleFunc.AddUObject(this, &UCPL_MGAttackFireRange::EndAttack);

	#pragma endregion

	#pragma region UI
	//@UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		SkillCastWidget = MainHUD->GetWidgetSkillCastingBar();
	}

	#pragma endregion

	// @Set MouseController
	MouseController = Player->GetPlayerCSMouseController();
	check(MouseController);
}

void UCPL_MGAttackFireRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// @마우스 대기상태
	if (bMouseWaiting == true)
	{
		if (MouseController != nullptr)
		{
			EMouseState MCState = MouseController->GetMouseState();
			if (MCState == EMouseState::CHECKINGPOINT)
			{
				//@Play
				SetPlayAfterMouseControl();
				MouseEndWaiting();
			}
			else if (MCState == EMouseState::NONE)
			{
				MouseEndWaiting();
			}
		}
	}

	IfFalseRet(bAttacking); //@클릭 후, 몽타주 실행되면서, Attacking 시작.

	// @스킬 시전 중..
	if (bFillingReadyGauge == true)
	{
		if (SkillCastWidget->GetFinishFilledProgressBar() == true)
		{
			bFillingReadyGauge = false;

			Player->ActorAnimMonPlay
			(
				MageAttackMontages[0],
				0.7f, true				// @AnimPlay 무조건 실행.
			);

			//@Jump Section
			Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Looping", MageAttackMontages[0]);

			//@ON Particle Object
			FireRainActor->OnStartActor(AttackPosition);
			FTransform Transform;
			Transform.SetLocation(FVector(0.0f, 0.0f, 500.0f));
			FireRainActor->SetParticleCompRelative(Transform);

			//@다시 줄어드는 Filling
			bFillOutGauge = true;
			SkillCastWidget->StartProgress(FillOutSpeed, 1.0f, true);
		}
	}

	// @이동 방향키 누르면, 공격 취소 - bPlayerInput
	APlayerController* controller = Cast<APlayerController>(Player->GetController());
	if (controller != nullptr /* && bAttacking == false */)
	{
		if (controller->IsInputKeyDown(EKeys::W)
			|| controller->IsInputKeyDown(EKeys::S)
			|| controller->IsInputKeyDown(EKeys::A)
			|| controller->IsInputKeyDown(EKeys::D))
		{
			//@StopMontage
			Player->ActorStopAnimMon(MageAttackMontages[0]);

			//@EndAttack
			EndAttackDeleFunc.Broadcast();
			return;
		}
	}

	//@ 공격 진행 중...
	//*Looping 이라, EndAttack 직접 시행
	if (bFillOutGauge == true)
	{
		// @PlayerInput 이 들어오거나, Progressbar 가 다 채워지면 End
		if (SkillCastWidget->GetFinishFilledProgressBar() == true)
		{
			//@StopMontage
			Player->ActorStopAnimMon(MageAttackMontages[0]);

			EndAttackDeleFunc.Broadcast();
		}
	}
}

void UCPL_MGAttackFireRange::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UCPL_MGAttackFireRange::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Mouse Controller ON
	MouseStartWaiting();
}

void UCPL_MGAttackFireRange::EndAttack()
{
	Super::EndAttack();

	SkillCastWidget->EndProgress();
	bMouseWaiting = false;
	bFillingReadyGauge = false;
	bFillOutGauge = false;

	//@StopMontage
	Player->ActorStopAnimMon(MageAttackMontages[0]);

	//@Off Particle Object
	FireRainActor->OffEndActor();

	Player->OffBlockAction();
}

void UCPL_MGAttackFireRange::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_MGAttackFireRange::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MGAttackFireRange::CheckProcedural()
{
	Super::CheckProcedural();
}

/* 마우스 클릭 이후의 행동 정의 */
void UCPL_MGAttackFireRange::SetPlayAfterMouseControl()
{
	//@Mouse Position
	AttackPosition = MouseController->GetClickPoint();

	//@클릭 지점 바라보기.
	UCFL_ActorAgainst::LookAtPoint(Player, AttackPosition);

	// @공격 실행 - BeginAttack
	BeginAttackDeleFunc.Broadcast();

	//@Play Montage
	Player->ActorAnimMonPlay
	(
		MageAttackMontages[0],
		0.2f, true				// @AnimPlay 무조건 실행.
	);

	//@Progress Bar
	if (bFillingReadyGauge == false)
	{
		SkillCastWidget->StartProgress(FillingReadySpeed);
		bFillingReadyGauge = true;
	}

	//@Montage Pause 수행 - NextAction 이전까지.
	float StartTime = 0.0f;
	float EndTime = 0.0f;
	MageAttackMontages[0]->GetSectionStartAndEndTime(0, StartTime, EndTime);
	EndTime *= MontagePauseOffset;

	MontagePauseDel.BindUFunction(this, FName("TimerMontagePause"));
	GetWorld()->GetTimerManager().SetTimer
	(
		MontagePauseTimer, MontagePauseDel, 1.0f, false, EndTime
	);
}

void UCPL_MGAttackFireRange::MouseStartWaiting()
{
	// @이동 키를 제외한 Action Key Block 하기.
	Player->OnBlockAction();

	// @Mouse Controller
	MouseController->OnUsingDecalMouseControl(FVector(400.0f), Player, 2000.0f);
	bMouseWaiting = true;

	bFillingReadyGauge = false;
	bFillOutGauge = false;
}

void UCPL_MGAttackFireRange::MouseEndWaiting()
{
	Player->OffBlockAction();
	bMouseWaiting = false;
}

/*
* ProgressBar(스킬 시전) 이 완료될때까지 ,
* "시전동작을 마치고" ,  NextAction 들어가기 전까지 Montage 를 멈춤.
*/
void UCPL_MGAttackFireRange::TimerMontagePause()
{
	Player->GetMesh()->GetAnimInstance()->Montage_Pause(MageAttackMontages[0]);
}
