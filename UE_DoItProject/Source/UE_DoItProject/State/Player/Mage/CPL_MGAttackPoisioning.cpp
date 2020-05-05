#include "CPL_MGAttackPoisioning.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_GameMode/CBaseGameMode.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Projectile/CProjectile_PoisionBall.h"

//Widget
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_SkillCastingBar.h"

UCPL_MGAttackPoisioning::UCPL_MGAttackPoisioning()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;

		AttackRange = 1000.0f;
	}

	FString Path = L"";

	#pragma region MageAttackMontage
	// MageAttackMontage
	{
		UAnimMontage* Mage_Poisioning = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_Skilling_Poisioning.Mage_Skilling_Poisioning'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MPoisioning(*Path);
		if (MPoisioning.Succeeded())
			Mage_Poisioning = MPoisioning.Object;

		MageAttackMontages.Emplace(Mage_Poisioning);
	}

	#pragma endregion
}

void UCPL_MGAttackPoisioning::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bFillingGauge = false;

		//@Timer 무효화
		MontagePauseTimer.Invalidate();

		//@EndProgress
		SkillCastWidget->EndProgress();
	});

	// @EndAttack Delegate
	EndAttackDeleFunc.AddLambda([&]()
	{
		bFillingGauge = false;

		//@Timer 무효화
		MontagePauseTimer.Invalidate();

		//@EndProgress
		SkillCastWidget->EndProgress();

	});

	#pragma endregion


	//@UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		SkillCastWidget = MainHUD->GetWidgetSkillCastingBar();
	}
}

void UCPL_MGAttackPoisioning::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFillingGauge == true)
	{
		if (SkillCastWidget->GetFinishFilledProgressBar() == true)
		{
			bFillingGauge = false;

			Player->ActorAnimMonPlay
			(
				MageAttackMontages[0],
				1.0f, true				// @AnimPlay 무조건 실행.
			);

			//@Jump Section
			Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection("NextAction", MageAttackMontages[0]);
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
	}//(bFillingGauge == true)
}

void UCPL_MGAttackPoisioning::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCPL_MGAttackPoisioning::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick 에서 처리 중.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	if (Target == nullptr)
	{
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	// @타겟 바라보게 하기
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @Projectile 설정. -> Montage 에서 Spawn
	FVector Dir = Target->GetActorLocation() - Player->GetActorLocation();
	Dir.Normalize();
	float Speed = 2000.0f;
	FProjectileData ProjectileData;
	ProjectileData.Direction = Dir;
	ProjectileData.MoveSpeed = Speed;
	ProjectileData.FollowingTarget = Target;

	ACProjectile_PoisionBall::SettingProjectile(ProjectileData);

	// @공격 실행
	if (bAttacking == false)
	{
		//@Play Montage
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0],
			0.2f, true				// @AnimPlay 무조건 실행.
		);

		//@Progress Bar
		if (bFillingGauge == false)
		{
			SkillCastWidget->StartProgress(FillingSpeed);
			bFillingGauge = true;
		}

		//@Montage Pause 수행 - NextAction 이전까지.
		float StartTime = 0.0f;
		float EndTime = 0.0f;
		MageAttackMontages[0]->GetSectionStartAndEndTime(0, StartTime, EndTime);
		//EndTime *= MontagePauseOffset;

		MontagePauseDel.BindUFunction(this, FName("TimerMontagePause"));
		GetWorld()->GetTimerManager().SetTimer
		(
			MontagePauseTimer, MontagePauseDel, 1.0f, false, EndTime
		);
	}

}

void UCPL_MGAttackPoisioning::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_MGAttackPoisioning::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MGAttackPoisioning::CheckProcedural()
{
	Super::CheckProcedural();
}

/*
* ProgressBar(스킬 시전) 이 완료될때까지 ,
* "시전동작을 마치고" ,  NextAction 들어가기 전까지 Montage 를 멈춤.
*/
void UCPL_MGAttackPoisioning::TimerMontagePause()
{
	Player->GetMesh()->GetAnimInstance()->Montage_Pause(MageAttackMontages[0]);
}
