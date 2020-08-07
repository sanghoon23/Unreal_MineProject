#include "CHM_AssaThirdAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_Assassin.h"
#include "Notify/Event/CN_SpawnProjectile.h"

UCHM_AssaThirdAttack::UCHM_AssaThirdAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<uint8>(EHM_AssaThirdComboType::NONE);
		MaxComboNum = static_cast<uint8>(EHM_AssaThirdComboType::END);
	}

	FString Path = L"";

#pragma region FirstAttack

	//@ADD Attack Montage
	{
		UAnimMontage* Assa_ThirdAttackMon_01 = nullptr;
		UAnimMontage* Assa_ThirdAttackMon_02 = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_ThirdAttack01.HM_Assassin_Mon_ThirdAttack01'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MThirdAttack_01(*Path);
		if (MThirdAttack_01.Succeeded())
			Assa_ThirdAttackMon_01 = MThirdAttack_01.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_ThirdAttack02.HM_Assassin_Mon_ThirdAttack02'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MThirdAttack_02(*Path);
		if (MThirdAttack_02.Succeeded())
			Assa_ThirdAttackMon_02 = MThirdAttack_02.Object;

		AttackMontages.Emplace(Assa_ThirdAttackMon_01);
		AttackMontages.Emplace(Assa_ThirdAttackMon_02);
	}

#pragma endregion

}

void UCHM_AssaThirdAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_Assassin);
	check(I_Charactor);

	//@Notify Reference
	TArray<FAnimNotifyEventReference> NotifyEvent_Mon_0;
	AttackMontages[0]->GetAnimNotifies(0, 15.0f, false, NotifyEvent_Mon_0);
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

void UCHM_AssaThirdAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssaThirdAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaThirdAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	/* EquipComp 없다 */
	//IfTrueRet(HM_Assassin->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	//@Notify Ref - Setting Target
	AActor* TargetInAI = HM_Assassin->GetTargetInAI();
	for (auto& Notify : Notifies_SpawnProjectile)
	{
		Notify->SpeedValue = 800.0f;
		if (TargetInAI != nullptr)
		{
			Notify->SetProjectileDirection(TargetInAI->GetActorLocation() - HM_Assassin->GetActorLocation());
			Notify->SetProjectileTarget(TargetInAI);
		}
		else
		{
			Notify->SetProjectileDirection(HM_Assassin->GetActorForwardVector());
		}
	}

	if (bAttacking == false)
	{
		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}

void UCHM_AssaThirdAttack::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Pawn = Cast<IIC_Charactor>(DoingActor);
	check(Pawn);

	//@#Edit 0605
	//bComboCheck = false;
	++CurrentComboNum;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Pawn->ActorAnimMonPlay
		(
			AttackMontages[CurrentComboNum],
			0.9f, true
		);
	}
	else
	{
		EndAttack();
	}
}

//bool UCHM_AssaThirdAttack::IsLastCombo() const
//{
//	if (CurrentComboNum == static_cast<UINT>(EHM_AssaThirdComboType::COMBO_TWO))
//		return true;
//
//	return false;
//}
//
///*
//#Edit 0701 -
//부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
//Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
//ex) 첫번째 공격 1, 두번째 공격 2...
//*/
//void UCHM_AssaThirdAttack::AttackOtherPawn()
//{
//	Super::AttackOtherPawn();
//}
