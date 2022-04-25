#include "CHM_AssaFirstAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_Assassin.h"

UCHM_AssaFirstAttack::UCHM_AssaFirstAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Assa_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_FirstAttack.HM_Assassin_Mon_FirstAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Assa_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Assa_FirstAttack);
	}

#pragma endregion

}

void UCHM_AssaFirstAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_Assassin);
	check(I_Charactor);
}

void UCHM_AssaFirstAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssaFirstAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaFirstAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	/* EquipComp ���� */
	//IfTrueRet(HM_Assassin->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, true
		);
	}
}

/*
#Edit 0701 -
�θ𿡼� EndAttack �� CurrentComboNum �� 0 ���� �����ֱ� ������,
Enum - NONE �� ���Խ��Ѽ� �޺� �ε����� ���߾����.
ex) ù��° ���� 1, �ι�° ���� 2...
*/
void UCHM_AssaFirstAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

	//@���� �޺� �÷���
	++CurrentComboNum;
	const uint8 ComboNum = static_cast<uint8>(CurrentComboNum);

	IfTrueRet(HM_Assassin == nullptr);

	FVector ActorForward = HM_Assassin->GetActorForwardVector();
	FVector Start = HM_Assassin->GetActorLocation();
	FVector End = HM_Assassin->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_Assassin);

	TArray<FHitResult> HitResults;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepMultiByChannel //@Single - ����.
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

	//DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		for (FHitResult& HitResult : HitResults)
		{
			IIC_Charactor* HitIneterfaceCharactor = Cast<IIC_Charactor>(HitResult.GetActor());
			if (HitIneterfaceCharactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* I_HitComp = HitIneterfaceCharactor->GetIHitComp();
				if (I_HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					//@���� ���� �������� HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_Assassin->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

					//@DT_Normal
					I_HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());
					I_HitComp->OnHit(HM_Assassin, DamageType, DamageType->DamageImpulse);
				}
				else
					UE_LOG(LogTemp, Warning, L"MaoFirstAttack CallAttack - HitComp Null!!");

			}//(Charactor)
			else
				UE_LOG(LogTemp, Warning, L"MaoFirstAttack CallAttack - Charactor Null!!");
		}
	}
}
