#include "CHM_BasicFirstCombo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Monster/CHM_Basic.h"
#include "Interface/IC_Charactor.h"

UCHM_BasicFirstCombo::UCHM_BasicFirstCombo()
{
	PrimaryComponentTick.bCanEverTick = true;

	// @Combo ONE - 해당 콤보 첫번째 Index 공격
	CurrentComboNum = static_cast<UINT>(UHM_BasicFirstComboType::COMBO_ONE);
	MaxComboNum = static_cast<UINT>(UHM_BasicFirstComboType::END);

#pragma region BasicAttack

	UAnimMontage* HM_FirstAttack_1 = nullptr;
	UAnimMontage* HM_FirstAttack_2 = nullptr;
	UAnimMontage* HM_FirstAttack_3 = nullptr;

	FString Path = L"";
	// Setting
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_BasicAttack1.HM_Basic_BasicAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			HM_FirstAttack_1 = MBasicAttack_1.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_BasicAttack2.HM_Basic_BasicAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			HM_FirstAttack_2 = MBasicAttack_2.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_BasicAttack3.HM_Basic_BasicAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			HM_FirstAttack_3 = MBasicAttack_3.Object;

		AttackMontages.Emplace(HM_FirstAttack_1);
		AttackMontages.Emplace(HM_FirstAttack_2);
		AttackMontages.Emplace(HM_FirstAttack_3);
	}

#pragma endregion
}

void UCHM_BasicFirstCombo::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_BasicFirstCombo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_BasicFirstCombo::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	ACHM_Basic* Pawn = Cast<ACHM_Basic>(DoingActor);
	check(Pawn);

	// @IF TRUE RETURN
	IfTrueRet(Pawn->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Pawn->GetIEquipComp()->GetEquiping()); //@Equping Check

	//@IsLastCombo - 
	// AI 에서 BeginAttack 을 계속 호출하기 때문에,
	// Player 랑 다르게 제어가 필요하다.
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		Pawn->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			1.3f, false
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCHM_BasicFirstCombo::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Pawn = Cast<IIC_Charactor>(DoingActor);
	check(Pawn);

	bComboCheck = false;
	++CurrentComboNum;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Pawn->ActorAnimMonPlay
		(
			AttackMontages[CurrentComboNum],
			1.3f, true
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCHM_BasicFirstCombo::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(UHM_BasicFirstComboType::COMBO_THREE))
		return true;

	return false;
}

