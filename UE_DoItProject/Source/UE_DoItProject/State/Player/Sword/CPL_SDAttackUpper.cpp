#include "CPL_SDAttackUpper.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "Component/Player/CPL_BlendCameraComp.h"

UCPL_SDAttackUpper::UCPL_SDAttackUpper()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Super
	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(USD_UpperAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(USD_UpperAttack::END);

		AttackRange = 230.0f;
	}
	#pragma endregion

	FString Path = L"";
	#pragma region Sword UpperAttack Montages
	// Sword UpperAttack Montages
	{
		// @1
		UAnimMontage* Sword_UpperAttack_1 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack1.SwordMon_UpperAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_1(*Path);
		if (MSecondAttack_1.Succeeded())
			Sword_UpperAttack_1 = MSecondAttack_1.Object;

		// @2
		UAnimMontage* Sword_UpperAttack_2 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack2.SwordMon_UpperAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_2(*Path);
		if (MSecondAttack_2.Succeeded())
			Sword_UpperAttack_2 = MSecondAttack_2.Object;

		// @3
		UAnimMontage* Sword_UpperAttack_3 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack3.SwordMon_UpperAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_3(*Path);
		if (MSecondAttack_3.Succeeded())
			Sword_UpperAttack_3 = MSecondAttack_3.Object;

		// @4
		UAnimMontage* Sword_UpperAttack_4 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack4.SwordMon_UpperAttack4'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_4(*Path);
		if (MSecondAttack_4.Succeeded())
			Sword_UpperAttack_4 = MSecondAttack_4.Object;

		// @5
		UAnimMontage* Sword_UpperAttack_5 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack5.SwordMon_UpperAttack5'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_5(*Path);
		if (MSecondAttack_5.Succeeded())
			Sword_UpperAttack_5 = MSecondAttack_5.Object;

		// @6
		UAnimMontage* Sword_UpperAttack_6 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack6.SwordMon_UpperAttack6'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_6(*Path);
		if (MSecondAttack_6.Succeeded())
			Sword_UpperAttack_6 = MSecondAttack_6.Object;

		SwordAttackMontages.Emplace(Sword_UpperAttack_1);
		SwordAttackMontages.Emplace(Sword_UpperAttack_2);
		SwordAttackMontages.Emplace(Sword_UpperAttack_3);
		SwordAttackMontages.Emplace(Sword_UpperAttack_4);
		SwordAttackMontages.Emplace(Sword_UpperAttack_5);
		SwordAttackMontages.Emplace(Sword_UpperAttack_6);
	}

	#pragma endregion

	#pragma region DamageType

	DT_Air			= NewObject<UCDamageType_Air>();
	DT_AirAttack	= NewObject<UCDamageType_AirAttack>();
	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion

}

void UCPL_SDAttackUpper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackUpper::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion

	PlayerController = Cast<APlayerController>(Player->GetController());
	check(PlayerController);

	// @EndAttack Delegate - BlendCamera
	EndAttackDeleFunc.AddLambda([&]()
	{
		//@Target NULL
		//Target = nullptr;

		//@���� �ð� ���󺹱�.
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	});
}

void UCPL_SDAttackUpper::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

// - IBaseAttack ����.
// @param DoingActor - �����ϴ� ��ü
/* 
@Warning 
 #Edit *0220 - ���� Anim �� ������ true �� ����
���������� InputKey �� ���ö�, ������ ��Ÿ�ְ� ������ ��Ÿ���� bAttacking == false �� ����.(ComboNotify �� EndAttack ��)
 #Edit *0312, CameraActor Blend �ϱ� ���ؼ� Delegate - CutOutBlendCameraFunc(�߰�), LastOutBlendCameraFunc(����)
*/
void UCPL_SDAttackUpper::BeginAttack(AActor * DoingActor)
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
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	//@ Ÿ�� �ٶ󺸰� �ϱ�
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.7f;

	if (bAttacking == false)
	{
		bool bInAir = false;
		ACharacter* CharactorTarget = Cast<ACharacter>(Target);
		if (CharactorTarget != nullptr)
		{
			//@���߿� ������ ����������, 
			// �������� �̻�( AirComboCanHeight )  �̸� �ι�° ����(���ӱ�) ���� ������.
			if (UCFL_ActorAgainst::IsTargetInAir(CharactorTarget) == true)
			{
				bInAir = true;
				if (Target->GetActorLocation().Z > AirComboCanHeight)
				{
					BeginAttackDeleFunc.Broadcast();
					OnComboSet(Player);

					return;
				}

				AttackDecision->StopAttackTrace();
				Player->ActorStopAnimMon(SwordAttackMontages[0]);
			}
		}

		if (bInAir == false)
		{
			//@�⺻ ����
			Player->ActorAnimMonPlay
			(
				SwordAttackMontages[0], /* @FirstMontage == Combo1 */
				1.3f, true				// @AnimPlay ������ ����.
			);
		}//(bInAir==false)
	}//(bAttacking==false)
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}

	//@Delegate ����
	if (CutOutBlendCameraFunc.IsValid() == false)
	{
		CutOutBlendCameraFunc = EndAttackDeleFunc.AddUObject(this, &UCPL_SDAttackUpper::BlendCameraFunc);
	}

	//@Delegate ����
	EndAttackDeleFunc.Remove(LastOutBlendCameraFunc);
	LastOutBlendCameraFunc.Reset();

}

// - IBaseAttack ����.
/*
@Warning
 #Edit *0312, CameraActor Blend �ϱ� ���ؼ� 
������ ���� COMBO_SIX �κп� Delegate - CutOutBlendCameraFunc(����), LastOutBlendCameraFunc(�߰�)
*/
// @Warning - Target �� ������ �����ϰ� ����. - Finish Attack �� ����
// �� ������ ���߿� Target�� ESC �ؼ� nullptr �� �ǹ����� ����ǰԲ� ��.
void UCPL_SDAttackUpper::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	//APawn* Target = Player->GetFindAttackTarget();
	//check(Target);

	APawn* Target = Player->GetFindAttackTarget();
	//Target = Player->GetFindAttackTarget();
	if (Target == nullptr)
	{
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	//@���߿� ������ Ȯ��,
	ACharacter* CharactorTarget = Cast<ACharacter>(Target);
	if (CharactorTarget != nullptr)
	{
		if (UCFL_ActorAgainst::IsTargetInAir(CharactorTarget) == false)
		{
			EndAttackDeleFunc.Broadcast();
			return;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bComboCheck = false;
	++CurrentComboNum;

	//COMBO_TWO
	if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_TWO))
	{
		// @�Ÿ� ������, ���� ���߱� - Combo �� Target ��ġ �տ��� �����ϱ�
		UCFL_ActorAgainst::ActorLocateFrontTarget(Target, Player, AttackRange, true);

		// @�߷� ����
		Charactor->OffGravity();

		// @�ӷ� ���̱� - �߷²��� �ٷ� �������
		Player->GetCharacterMovement()->Velocity = FVector(0.0f);

		//@ī�޶� ��ȯ
		AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(EBlendCameraPositionType::BottomFace);
		if (BlendCameraActor != nullptr)
		{
			PlayerController->SetViewTargetWithBlend(BlendCameraActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, L"UpperAttack ComboSet(COMBO_TWO) - BlendCamera Null!!")
		}
	}
	//COMBO_SIX
	else if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
	{
		//@���� �ð� ����.
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4f);

		//@ī�޶� ��ȯ
		AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(EBlendCameraPositionType::ForwardFace);
		if (BlendCameraActor != nullptr)
		{
			Player->OnBlockKeyInput(); //@OnBlockInput
			PlayerController->SetViewTargetWithBlend(BlendCameraActor, 2.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, L"UpperAttack ComboSet(COMBO_SIX) - BlendCamera Null!!")
		}

		//@Delegate ����
		EndAttackDeleFunc.Remove(CutOutBlendCameraFunc);
		CutOutBlendCameraFunc.Reset();

		//@Delegate �߰�
		if (LastOutBlendCameraFunc.IsValid() == false)
		{
			LastOutBlendCameraFunc 
				= EndAttackDeleFunc.AddUObject(this, &UCPL_SDAttackUpper::EndAttackBlendCameraFunc);
		}
	}

	// @Input ����
	Player->CanNotMove();

	// @Ÿ�� �ٶ󺸰� �ϱ�
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.2f;

	// ���� �˻�.( CurrentComboNum = 0 ~ MaxCombo ���� )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Charactor->ActorAnimMonPlay
		(
			SwordAttackMontages[CurrentComboNum],
			1.8f, false
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�.
bool UCPL_SDAttackUpper::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
		return true;

	return false;
}


/* �ٸ� Pawn �� ���� ó�� �Լ� */
// @DoingActor - Attack �� �� ��ü ��, ���⼱ Player (Owner)
void UCPL_SDAttackUpper::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	TArray<FHitResult> HitResults;
	float DebugLifeTime = 1.0f;
	//#Edit 0610 - Ư�� ������ ������ ��� ������ ���� ����
	bool bHit = GetWorld()->SweepMultiByChannel
	(
		HitResults
		, Start
		, End
		, FQuat::Identity
		, ECC_GameTraceChannel2 // @PlayerAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		for (FHitResult& HitResult : HitResults)
		{
			IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
			if (Charactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* HitComp = Charactor->GetIHitComp();
				if (HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					FVector HitDirection = Player->GetActorForwardVector();
					HitDirection.Z = 0.0f;
					HitComp->SetHitDirection(HitDirection);

					// 1.2 Hit Delegate - Air(DamageType)
					if (CurrentComboNum == 0)
					{
						HitComp->SetHitMoveSpeed(0.3f);
						HitComp->OnHit(Player, DT_Air, 5.0f);
					}
					else if (CurrentComboNum > 0 && CurrentComboNum < static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
					{
						HitComp->SetHitMoveSpeed(0.3f);
						HitComp->OnHit(Player, DT_AirAttack, 5.0f);
					}
					else if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
					{
						HitComp->SetHitMoveSpeed(500.0f);
						HitComp->OnHit(Player, DT_StrongAttack, 5.0f);
					}

				}//(HitComp != nullptr)
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
			}//(Charactor != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
		}//for(HitResults)
	}//(bHit == true)
}

/* COMBO �������� ������ ���� CameraBlend ó�� */
void UCPL_SDAttackUpper::BlendCameraFunc()
{
	PlayerController->SetViewTargetWithBlend(Player);
}

/* COMBO �� ������ ������ CameraBlend ó�� */
void UCPL_SDAttackUpper::EndAttackBlendCameraFunc()
{
	GetWorld()->GetTimerManager().SetTimer(EndBlendTimerHandle, this, &UCPL_SDAttackUpper::TimerFunc, 2.0f);
}

/* COMBO �� ������ ���� SetTimer Binding Function.*/
void UCPL_SDAttackUpper::TimerFunc()
{
	Player->OffBlockKeyInput();
	PlayerController->SetViewTargetWithBlend(Player);
}

