#include "CHM_MaoFirstAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Charactor/Monster/CHM_PengMao.h"

UCHM_MaoFirstAttack::UCHM_MaoFirstAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	AttackRangeVec.Init(0.0f, static_cast<uint8>(EHM_MaoFirstComboType::END));
	AttackRadiusVec.Init(0.0f, static_cast<uint8>(EHM_MaoFirstComboType::END));

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Mao_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Attack/PenMaoAnim_FirstAttack_Montage.PenMaoAnim_FirstAttack_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Mao_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Mao_FirstAttack);
	}

#pragma endregion
}

void UCHM_MaoFirstAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_PengMao);
	check(I_Charactor);

#pragma region Create DamageType

	DT_Noraml = NewObject<UCDamageType_Normal>();
	DT_Noraml->SetDamageImpulse(10.0f);

	DT_Strong = NewObject<UCDamageType_StrongAttack>();
	DT_Strong->SetDamageImpulse(20.0f);

#pragma endregion

	//@Setting Value
	{
		OffSetAttackRangeForStart = 100.0f; //@Range - 100.0f

		//@공격 거리(Sphere 가 움직일 범위)
		AttackRangeVec[1] = 200.0f;
		AttackRangeVec[2] = 200.0f;
		AttackRangeVec[3] = 200.0f;
		AttackRangeVec[4] = 200.0f;

		//@공격 둘레 - Sphere
		AttackRadiusVec[1] = 200.0f;
		AttackRadiusVec[2] = 200.0f;
		AttackRadiusVec[3] = 200.0f;
		AttackRadiusVec[4] = 300.0f;
	}
}

void UCHM_MaoFirstAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_MaoFirstAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_MaoFirstAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_PengMao->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(HM_PengMao->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_PengMao->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}

bool UCHM_MaoFirstAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_MaoFirstComboType::COMBO_FOUR))
		return true;

	return false;
}

/*
#Edit 0701 -
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_MaoFirstAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	//@현재 콤보 늘려줌
	++CurrentComboNum;
	const uint8 ComboNum = static_cast<uint8>(CurrentComboNum);

	FVector ActorForward = HM_PengMao->GetActorForwardVector();
	FVector Start = HM_PengMao->GetActorLocation() + ActorForward * OffSetAttackRangeForStart;
	FVector End = HM_PengMao->GetActorLocation() + ActorForward * AttackRangeVec[ComboNum];

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadiusVec[ComboNum]);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_PengMao);

	TArray<FHitResult> HitResults;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepMultiByChannel //@Single - 단일.
	(
		HitResults
		, Start
		, End
		, FQuat::Identity
		, I_Charactor->GetCharactorUsingAttackChannel() // @MonsterAttack
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
				IIC_HitComp* I_HitComp = Charactor->GetIHitComp();
				if (I_HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_PengMao->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);
					//I_HitComp->SetHitMoveSpeed(0.3f);

					if ((IsLastCombo() == true))
					{
						I_HitComp->SetHitMoveSpeed(3.0f);
						I_HitComp->OnHit(HM_PengMao, DT_Strong, DT_Strong->DamageImpulse);

						//@Set Delegate
						I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_MaoFirstAttack::BeginBeatedFunction);
						I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_MaoFirstAttack::EndBeatedFunction);
					}
					else
					{
						I_HitComp->SetHitMoveSpeed(1.5f);
						I_HitComp->OnHit(HM_PengMao, DT_Noraml, DT_Noraml->DamageImpulse);
					}

					// 1.2 Hit Delegate - Normal(DamageType)
					//(IsLastCombo() == true)
					//	? I_HitComp->OnHit(HM_PengMao, DT_Noraml, DT_Noraml->DamageImpulse)
					//	: I_HitComp->OnHit(HM_PengMao, DT_Strong, DT_Strong->DamageImpulse);
					//HitComp->OnHit(HM_PengMao, DT_Noraml, 5.0f);
				}
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
			}
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_MaoFirstAttack::BeginBeatedFunction(AActor * Subject)
{
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Subject);
	if (I_Charactor != nullptr)
	{
		I_Charactor->CanNotMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Charactor NULL!!");

	IIC_Player* I_Player = Cast<IIC_Player>(Subject);
	if (I_Player != nullptr)
	{
		I_Player->OnBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Player NULL!!");

	//IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
	//if (I_HitComp != nullptr)
	//{

	//}
}

void UCHM_MaoFirstAttack::EndBeatedFunction(AActor * Subject)
{
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Subject);
	if (I_Charactor != nullptr)
	{
		I_Charactor->CanMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack EndBeatedFunc, I_Charactor NULL!!");

	IIC_Player* I_Player = Cast<IIC_Player>(Subject);
	if (I_Player != nullptr)
	{
		I_Player->OffBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack EndBetedFunc, I_Player NULL!!");
}
