#include "CPL_MGAttackFreezing.h"
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

#include "Actor/Projectile/CProjectile_FreezeBall.h"
#include "Notify/Event/CN_SpawnProjectile.h"

//Widget
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_SkillCastingBar.h"

UCPL_MGAttackFreezing::UCPL_MGAttackFreezing()
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
		UAnimMontage* Mage_Freezing = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_Skilling_FreezeBall.Mage_Skilling_FreezeBall'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFreezingAttack(*Path);
		if (MFreezingAttack.Succeeded())
			Mage_Freezing = MFreezingAttack.Object;

		MageAttackMontages.Emplace(Mage_Freezing);
	}
	#pragma endregion
}

void UCPL_MGAttackFreezing::BeginPlay()
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

		//@Timer ��ȿȭ
		MontagePauseTimer.Invalidate();

		//@EndProgress
		SkillCastWidget->EndProgress();
	});

	// @EndAttack Delegate
	EndAttackDeleFunc.AddLambda([&]()
	{
		bFillingGauge = false;

		//@Timer ��ȿȭ
		MontagePauseTimer.Invalidate();

		//@EndProgress
		SkillCastWidget->EndProgress();
	});

#pragma endregion

	//@UI
	//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//if (MainHUD != nullptr)
	//{
	//	AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
	//	check(MainHUD);
	//	SkillCastWidget = MainHUD->GetWidgetSkillCastingBar();
	//}

	check(MainHUD);
	SkillCastWidget = MainHUD->GetWidgetSkillCastingBar();

	//@Notify Reference
	TArray<FAnimNotifyEventReference> NotifyEvent_Mon_0;
	MageAttackMontages[0]->GetAnimNotifies(0, 10.0f, false, NotifyEvent_Mon_0);
	for (auto& Ref : NotifyEvent_Mon_0) //@AttackMontage[0]
	{
		const FAnimNotifyEvent* Event = Ref.GetNotify();
		UCN_SpawnProjectile* Notify = Cast<UCN_SpawnProjectile>(Event->Notify);
		if (Notify != nullptr)
		{
			Notifies_SpawnProjectile.Add(Notify);
		}
	}
}

void UCPL_MGAttackFreezing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
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
				1.0f, true				// @AnimPlay ������ ����.
			);

			//@Jump Section
			Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection("NextAction", MageAttackMontages[0]);
		}

		// @�̵� ����Ű ������, ���� ��� - bPlayerInput
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

				EndAttackDeleFunc.Broadcast();
				return;
			}
		}
	}//(bFillingGauge == true)
}

void UCPL_MGAttackFreezing::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCPL_MGAttackFreezing::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick ���� ó�� ��.
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
		//@
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	// @Ÿ�� �ٶ󺸰� �ϱ�
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	//@Notify Ref - Setting Target
	for (auto& Notify : Notifies_SpawnProjectile)
	{
		Notify->SpeedValue = 800.0f;
		Notify->SetProjectileDirection(Target->GetActorLocation() - Player->GetActorLocation());
		Notify->SetProjectileTarget(Target);
	}

	// @���� ����
	if (bAttacking == false)
	{
		//@Play Montage
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0],
			0.5f, true				// @AnimPlay ������ ����.
		);

		//@Progress Bar
		if (bFillingGauge == false)
		{
			SkillCastWidget->StartProgress(FillingSpeed);
			bFillingGauge = true;
		}

		//@Montage Pause ���� - NextAction ��������.
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

void UCPL_MGAttackFreezing::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_MGAttackFreezing::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MGAttackFreezing::CheckProcedural()
{
	Super::CheckProcedural();
}

/*
* ProgressBar(��ų ����) �� �Ϸ�ɶ����� ,
* "���������� ��ġ��" ,  NextAction ���� ������ Montage �� ����.
*/
void UCPL_MGAttackFreezing::TimerMontagePause()
{
	Player->GetMesh()->GetAnimInstance()->Montage_Pause(MageAttackMontages[0]);
}
