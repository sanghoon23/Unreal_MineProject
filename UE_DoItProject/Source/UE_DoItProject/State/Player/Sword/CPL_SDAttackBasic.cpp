#include "CPL_SDAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"
#include "Interface/IC_Charactor.h"

#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_SDAttackBasic::UCPL_SDAttackBasic()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentComboNum = static_cast<UINT>(USD_BasicAttack::COMBO_ONE);
	MaxComboNum = static_cast<UINT>(USD_BasicAttack::END);

	UAnimMontage* Sword_BasicAttack_1 = nullptr;
	UAnimMontage* Sword_BasicAttack_2 = nullptr;
	UAnimMontage* Sword_BasicAttack_3 = nullptr;

	FString Path = L"";
	// Setting
	{
		UINT Sword_Type = static_cast<UINT>(SwordAttackType::BASIC);

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack1.Sword_BasicAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			Sword_BasicAttack_1 = MBasicAttack_1.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack2.Sword_BasicAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			Sword_BasicAttack_2 = MBasicAttack_2.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack3.Sword_BasicAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			Sword_BasicAttack_3 = MBasicAttack_3.Object;

		SwordAttackMontages.Emplace(Sword_BasicAttack_1);
		SwordAttackMontages.Emplace(Sword_BasicAttack_2);
		SwordAttackMontages.Emplace(Sword_BasicAttack_3);
	}
}

void UCPL_SDAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackBasic::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_SDAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	check(Player);

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	IfTrueRet(IsLastCombo()); //@IsLastCombo

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.5f, false
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCPL_SDAttackBasic::OnComboSet(AActor * DoingActor)
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
			SwordAttackMontages[CurrentComboNum],
			1.5f, false
		);
	}
	else
	{
		EndAttack(DoingActor);
	}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_SDAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_BasicAttack::COMBO_THREE))
		return true;

	return false;
}
