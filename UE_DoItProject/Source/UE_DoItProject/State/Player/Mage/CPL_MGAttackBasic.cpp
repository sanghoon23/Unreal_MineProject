#include "CPL_MGAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"

UCPL_MGAttackBasic::UCPL_MGAttackBasic()
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

void UCPL_MGAttackBasic::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_MGAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MGAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	IfTrueRet(IsLastCombo()); //@IsLastCombo

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, false
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCPL_MGAttackBasic::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	bComboCheck = false;
	++CurrentComboNum;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[CurrentComboNum], 
			1.0f, false
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_MGAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(UMG_BasicAttack::COMBO_THREE))
		return true;

	return false;
}
