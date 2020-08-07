#include "CHM_AssaFourAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_Assassin.h"

UCHM_AssaFourAttack::UCHM_AssaFourAttack()
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
		UAnimMontage* Assa_FourAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_FourAttack.HM_Assassin_Mon_FourAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> Mon_FourAttack(*Path);
		if (Mon_FourAttack.Succeeded())
			Assa_FourAttack = Mon_FourAttack.Object;

		AttackMontages.Emplace(Assa_FourAttack);
	}

#pragma endregion

}

void UCHM_AssaFourAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_Assassin);
	check(I_Charactor);
}

void UCHM_AssaFourAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssaFourAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaFourAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}

/*
#Edit 0701 -
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_AssaFourAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCHM_AssaFourAttack::BeginBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanNotMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OnBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Player NULL!!")
}

void UCHM_AssaFourAttack::EndBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack EndBeatedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OffBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack EndBetedFunc, I_Player NULL!!");
}


