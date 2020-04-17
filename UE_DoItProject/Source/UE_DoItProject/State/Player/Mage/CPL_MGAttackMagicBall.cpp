#include "CPL_MGAttackMagicBall.h"
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
#include "Actor/Projectile/CProjectile_MagicBall.h"

//Widget
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_SkillCastingBar.h"

UCPL_MGAttackMagicBall::UCPL_MGAttackMagicBall()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum		= 1;

		AttackRange = 1000.0f;
	}

	FString Path = L"";

	#pragma region SwordAttackMontage
	// SwordAttackMontage
	{
		UAnimMontage* Sword_BackRangeAttack_1 = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_MagicBallAttack.Mage_MagicBallAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBallMagicAttack(*Path);
		if (MBallMagicAttack.Succeeded())
			Sword_BackRangeAttack_1 = MBallMagicAttack.Object;

		MageAttackMontages.Emplace(Sword_BackRangeAttack_1);
	}
	#pragma endregion

	//#pragma region Create DamageType

	//DT_Normal = NewObject<UCDamageType_Normal>();

	//#pragma endregion

}

void UCPL_MGAttackMagicBall::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		SkillCastWidget = MainHUD->GetWidgetSkillCastingBar();
	}

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		SkillCastWidget->EndProgress();
		bFillingGauge = false;
	});

	#pragma endregion
}

void UCPL_MGAttackMagicBall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
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
	}
}

void UCPL_MGAttackMagicBall::BeginAttack(AActor * DoingActor)
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
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	// @타겟 바라보게 하기
	UCFL_ActorAgainst::LookAtTarget(Target, Player);

	// @Projectile 설정.
	FVector Dir = Target->GetActorLocation() - Player->GetActorLocation();
	Dir.Normalize();
	float Speed = 2000.0f;
	ACProjectile_MagicBall::SettingProjectile(Dir, Speed);

	// @공격 실행
	if (bAttacking == false)
	{
		//@Play Montage
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0],
			0.5f, true				// @AnimPlay 무조건 실행.
		);

		//@Progress Bar
		if (bFillingGauge == false)
		{
			SkillCastWidget->StartProgress();
			bFillingGauge = true;
		}

		//@Montage Pause 수행 - NextAction 이전까지.
		float StartTime = 0.0f;
		float EndTime = 0.0f;
		MageAttackMontages[0]->GetSectionStartAndEndTime(0, StartTime, EndTime);
		EndTime *= 2.0f;

		MontagePauseDel.BindUFunction(this, FName("TimerMontagePause"));
		GetWorld()->GetTimerManager().SetTimer
		(
			MontagePauseTimer, MontagePauseDel, 1.0f, false, EndTime
		);
	}
	CLog::Print(L"MagicBall Attack Call!!");
}

void UCPL_MGAttackMagicBall::AttackOtherPawn()
{
	Super::AttackOtherPawn();

//	FVector ActorForward = Player->GetActorForwardVector();
//	FVector Position = Player->GetActorLocation();
//
//	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
//	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);
//
//	float DebugLifeTime = 1.0f;
//	TArray<FOverlapResult> OverlapResults;
//	bool bOverlap = GetWorld()->OverlapMultiByChannel //@Single - 단일.
//	(
//		OverlapResults
//		, Position
//		, FQuat::Identity
//		, ECC_GameTraceChannel2 // @PlayerAttack
//		, sphere
//		, CollisionQueryParm
//	);
//
//#if  ENABLE_DRAW_DEBUG
//
//	DrawDebugSphere(GetWorld(), Position, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);
//
//#endif // ENABLE_DRAW_DEBUG
//
//	if (bOverlap == true)
//	{
//		for (FOverlapResult& OverlapResult : OverlapResults)
//		{
//			IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
//			if (Charactor != nullptr)
//			{
//				// 1. Get Interface HitComp
//				IIC_HitComp* HitComp = Charactor->GetIHitComp();
//				if (HitComp != nullptr)
//				{
//					// 1.1 Set Hit Attribute
//					FVector PlayerLocation = Player->GetActorLocation();
//					FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - PlayerLocation;
//					HitDirection.Normalize();
//					HitDirection.Z = 0.0f;
//					HitComp->SetHitDirection(HitDirection);
//					HitComp->SetHitMoveSpeed(0.3f);
//
//					// 1.2 Hit Delegate - Air(DamageType)
//					HitComp->OnHit(Player, DT_Normal, 50.0f);
//				}
//				else
//					UE_LOG(LogTemp, Warning, L"MGAttackMagicBall CallAttack - HitComp Null!!");
//			}
//			else
//				UE_LOG(LogTemp, Warning, L"MGAttackMagicBall CallAttack - Charactor Null!!");
//		}
//	}
//
}

void UCPL_MGAttackMagicBall::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MGAttackMagicBall::CheckProcedural()
{
	Super::CheckProcedural();
}

ACProjectile_MagicBall* UCPL_MGAttackMagicBall::SpawnMagicBall()
{
	//Player->GetMesh()->GetAnimInstance()->Montage_GetPlayRate();

	FVector PlayerLocation = Player->GetActorLocation();
	PlayerLocation.Z += 15.0f;
	FVector PlayerForwardVec = Player->GetActorForwardVector();
	PlayerLocation += Player->GetActorForwardVector() * 50.0f;

	FTransform MagicBallTransform = FTransform::Identity;
	MagicBallTransform.SetLocation(PlayerLocation);
	FActorSpawnParameters Params;
	Params.Owner = Player;

	//@Spawn
	//UClass* test = ACProjectile_MagicBall::StaticClass();
	ACProjectile_MagicBall* MagicBall 
		= GetWorld()->SpawnActor<ACProjectile_MagicBall>(ACProjectile_MagicBall::StaticClass(), MagicBallTransform, Params);
	if (MagicBall != nullptr)
	{
		return MagicBall;
	}

	//@Setting
	//MagicBall->SettingProjectile(PlayerForwardVec);

	return nullptr;
}

/* 
* ProgressBar(스킬 시전) 이 완료될때까지 ,
* "시전동작을 마치고" ,  NextAction 들어가기 전까지 Montage 를 멈춤. 
*/
void UCPL_MGAttackMagicBall::TimerMontagePause()
{
	Player->GetMesh()->GetAnimInstance()->Montage_Pause(MageAttackMontages[0]);
}
