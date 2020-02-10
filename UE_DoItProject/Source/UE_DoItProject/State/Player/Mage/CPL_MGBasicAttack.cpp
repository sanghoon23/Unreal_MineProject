#include "CPL_MGBasicAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"
#include "Interface/IC_Charactor.h"

#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_MGBasicAttack::UCPL_MGBasicAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentComboNum = static_cast<UINT>(UMG_BasicAttack::COMBO_ONE);
	MaxComboNum = static_cast<UINT>(UMG_BasicAttack::END);

	#pragma region BasicAttack

	UAnimMontage* Mage_BasicAttack_1 = nullptr;
	UAnimMontage* Mage_BasicAttack_2 = nullptr;
	UAnimMontage* Mage_BasicAttack_3 = nullptr;

	FString Path = L"";
	// Setting
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack1.Mage_BasicAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			Mage_BasicAttack_1 = MBasicAttack_1.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack2.Mage_BasicAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			Mage_BasicAttack_2 = MBasicAttack_2.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack3.Mage_BasicAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			Mage_BasicAttack_3 = MBasicAttack_3.Object;

		MageAttackMontages.Emplace(Mage_BasicAttack_1);
		MageAttackMontages.Emplace(Mage_BasicAttack_2);
		MageAttackMontages.Emplace(Mage_BasicAttack_3);
	}

	#pragma endregion
}

void UCPL_MGBasicAttack::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_MGBasicAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MGBasicAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	check(Player);

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0], /* @FirstMontage == Combo1 */
			1.5f, false
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCPL_MGBasicAttack::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);

	IIC_Charactor* Player = Cast<IIC_Charactor>(DoingActor);
	check(Player);

	bComboCheck = false;
	++CurrentComboNum;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[CurrentComboNum], 
			1.5f, false
		);
	}
	else
	{
		EndAttack(DoingActor);
	}
}
