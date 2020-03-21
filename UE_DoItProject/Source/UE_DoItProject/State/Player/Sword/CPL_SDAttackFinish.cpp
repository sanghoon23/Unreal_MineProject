#include "CPL_SDAttackFinish.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "Component/Player/CPL_BlendCameraComp.h"

UCPL_SDAttackFinish::UCPL_SDAttackFinish()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(USD_FinalAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(USD_FinalAttack::END);

		AttackRange = 400.0f;
	}

	FString Path = L"";
	#pragma region Sword FinalAttack Montages
	// Sword FinalAttack Montages
	{
		// @1
		UAnimMontage* Sword_FinalAttack_1 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_FinishAttack_1.SwordMon_FinishAttack_1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFinalAttack_1(*Path);
		if (MFinalAttack_1.Succeeded())
			Sword_FinalAttack_1 = MFinalAttack_1.Object;

		// @2
		UAnimMontage* Sword_FinalAttack_2 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_FinishAttack_2.SwordMon_FinishAttack_2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFinalAttack_2(*Path);
		if (MFinalAttack_2.Succeeded())
			Sword_FinalAttack_2 = MFinalAttack_2.Object;

		// @3
		UAnimMontage* Sword_FinalAttack_3 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_FinishAttack_3.SwordMon_FinishAttack_3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFinalAttack_3(*Path);
		if (MFinalAttack_3.Succeeded())
			Sword_FinalAttack_3 = MFinalAttack_3.Object;

		SwordAttackMontages.Emplace(Sword_FinalAttack_1);
		SwordAttackMontages.Emplace(Sword_FinalAttack_2);
		SwordAttackMontages.Emplace(Sword_FinalAttack_3);
	}

	#pragma endregion

	#pragma region DamageType

	DT_Noraml		= NewObject<UCDamageType_Normal>();
	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion
}

void UCPL_SDAttackFinish::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackFinish::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Player->GetController());
	check(PlayerController);

	//@BeginAttack Delegate
	BeginAttackDeleFunc.AddLambda([&]()
	{
		//@ON BlockInput
		Player->OnBlockKeyInput();
	});

	//@EndAttak Delegate
	EndAttackDeleFunc.AddLambda([&]() 
	{
		//@���� ���󺹱�.
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		//@OFF BlockInput
		Player->OffBlockKeyInput();

		//@Camera ���󺹱�
		PlayerController->SetViewTargetWithBlend(Player);
	});
}

// - IBaseAttack ����.
// @param DoingActor - �����ϴ� ��ü
// #Edit *0310
// @Warning - 
// ��Ÿ�ָ� ������ ���� ����. �׷��� Player �� Input ���� ���
// ���������� �����ϱ� ���ؼ� �׻�, ComboCheck = true
void UCPL_SDAttackFinish::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		bAttackCall = true;
		IfFalseRet(bAttackPossible); // @Super::Tick ���� ó�� ��.
	}

	//@IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	// @Target �� ���� �ִٸ� ���� ��� �ߴ�.
	ACharacter* TargetCharactor = Cast<ACharacter>(Target);
	if (TargetCharactor != nullptr)
	{
		if (TargetCharactor->GetCharacterMovement()->IsFalling()
			&& TargetCharactor->GetCharacterMovement()->GravityScale <= 0.0f)
		{
			bAttackCall = false;
			return;
		}

		if (TargetCharactor->GetCharacterMovement()->IsFalling())
		{
			bAttackCall = false;
			return;
		}

	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	//@ī�޶� ��ȯ
	{
		AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(EBlendCameraPositionType::RightSideFace);
		if (BlendCameraActor != nullptr)
		{
			PlayerController->SetViewTargetWithBlend(BlendCameraActor);
		}
		else
			UE_LOG(LogTemp, Warning, L"FinalAttack BeginAttack(COMBO_ONE) - BlendCamera Null!!")
	}

	//@���� �ð� ������ ����.
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.7f);

	//@OFF AI - Target
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
	if (I_Monster != nullptr)
	{
		I_Monster->SetAIRunningPossible(false);
	}

	//@Ÿ�� �ٶ󺸰� �ϱ�
	LookAtTarget(Target);

	//@�Ÿ� ������, ���� ���߱�
	ActorLocateFrontTarget(Target);

	//@���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.7f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.1f, true				// @AnimPlay ������ ����.
		);
	}

	// @ComboCheck == true �� ��
	bComboCheck = true;
}

// - IBaseAttack ����.
// @Warning - bComboCheck = false ���� ����.
void UCPL_SDAttackFinish::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@++
	++CurrentComboNum;

	//COMBO_TWO
	if (CurrentComboNum == static_cast<uint8>(USD_FinalAttack::COMBO_TWO))
	{
		//@ī�޶� ��ȯ
		AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(EBlendCameraPositionType::LeftSideFace);
		if (BlendCameraActor != nullptr)
		{
			PlayerController->SetViewTargetWithBlend(BlendCameraActor);
		}
		else
			UE_LOG(LogTemp, Warning, L"FinalAttack ComboSet(COMBO_TWO) - BlendCamera Null!!")

		//@���� �ð� ������ ����.
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}

	//COMBO_THREE
	else if (CurrentComboNum == static_cast<uint8>(USD_FinalAttack::COMBO_THREE))
	{
		//@ī�޶� ��ȯ
		AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(EBlendCameraPositionType::ForwardFace);
		if (BlendCameraActor != nullptr)
		{
			PlayerController->SetViewTargetWithBlend(BlendCameraActor, 2.0f);
		}
		else
			UE_LOG(LogTemp, Warning, L"FinalAttack ComboSet(COMBO_THREE) - BlendCamera Null!!")

		//@���� �ð� ������ ����.
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);

		//@ON AI - Target
		IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
		if (I_Monster != nullptr)
		{
			I_Monster->SetAIRunningPossible(true);
		}
	}

	// @Input ����
	Player->CanNotMove();

	// @Ÿ�� �ٶ󺸰� �ϱ�
	LookAtTarget(Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 1.2f;

	// ���� �˻�.( CurrentComboNum = 0 ~ MaxCombo ���� )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Charactor->ActorAnimMonPlay
		(
			SwordAttackMontages[CurrentComboNum],
			1.2f, false
		);
	}
	else
	{
		EndAttack();
	}
}

/* @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�. */
bool UCPL_SDAttackFinish::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_FinalAttack::COMBO_THREE))
		return true;

	return false;
}


/* �ٸ� Pawn �� ���� ó�� �Լ� */
// @DoingActor - Attack �� �� ��ü ��, ���⼱ Player (Owner)
void UCPL_SDAttackFinish::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	FHitResult HitResult;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepSingleByChannel //@Single - ����.
	(
		HitResult
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

				// 1.2 Hit Delegate - (DamageType)
				if (CurrentComboNum != static_cast<uint8>(USD_FinalAttack::COMBO_THREE))
				{
					HitComp->SetHitMoveSpeed(0.3f);
					HitComp->OnHit(Player, DT_Noraml, 50.0f);
				}
				else
				{
					HitComp->SetHitMoveSpeed(0.3f);
					HitComp->OnHit(Player, DT_StrongAttack, 50.0f);
				}

			}//(HitComp != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
		}//(Charactor != nullptr)
		else
			UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
	}//(bHit == true)
}

/* TargetSystem Target �� �ٶ󺸵��� �� */
void UCPL_SDAttackFinish::LookAtTarget(AActor* Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

/* TargetSystem Target �� ���� �� Player �� ���̸� �����ϰ� ����. */
// @Warning - StartAttackActionDistance(Attack �� ���۵Ǵ� ��ġ)
// AttackRange �� �ٸ� Value ��.
void UCPL_SDAttackFinish::ActorLocateFrontTarget(AActor * Target)
{
	check(Target);
	FVector TargetLocation = Target->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	FVector LookDir = TargetLocation - PlayerLocation;
	LookDir.Normalize();
	LookDir.Z = 0.0f;

	// @Target ���� AttackRange ��ŭ ������ ������ ��ġ��Ű�� ����
	FVector SettingLocation = TargetLocation;

	// *StartAttackActionDistance*
	SettingLocation += (-1) * LookDir * (StartAttackActionDistance + 0.0f);

	// @Target �� ���̿� ���߰�,
	SettingLocation.Z = TargetLocation.Z;

	// @Setting Location
	Player->SetActorLocation(SettingLocation);
}
