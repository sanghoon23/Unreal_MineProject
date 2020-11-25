#include "CHM_AssaSecondAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_Assassin.h"

UCHM_AssaSecondAttack::UCHM_AssaSecondAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<uint8>(EHM_AssaSecondComboType::NONE);
		MaxComboNum = static_cast<uint8>(EHM_AssaSecondComboType::END);
	}

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Assa_SecondAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_SecondAttack.HM_Assassin_Mon_SecondAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack(*Path);
		if (MSecondAttack.Succeeded())
			Assa_SecondAttack = MSecondAttack.Object;

		AttackMontages.Emplace(Assa_SecondAttack);
	}

#pragma endregion

}

void UCHM_AssaSecondAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_Assassin);
	check(I_Charactor);
}

void UCHM_AssaSecondAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssaSecondAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaSecondAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	// @IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	/* EquipComp 없다 */
	//IfTrueRet(HM_Assassin->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, true
		);

		//@이동
		AttackMoveDir = HM_Assassin->GetActorForwardVector();
		AttackMoveSpeed = 10.0f;
	}
}

bool UCHM_AssaSecondAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_AssaSecondComboType::COMBO_SIX))
		return true;

	return false;
}

/*
#Edit 0701 -
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_AssaSecondAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);

	IfTrueRet(HM_Assassin == nullptr);

	//@타겟 바라보기
	AActor* AITarget = HM_Assassin->GetTargetInAI();
	check(AITarget);
	UCFL_ActorAgainst::LookAtTarget(HM_Assassin, AITarget);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@현재 콤보 늘려줌
	++CurrentComboNum;
	const uint8 ComboNum = static_cast<uint8>(CurrentComboNum);

	FVector ActorForward = HM_Assassin->GetActorForwardVector();
	FVector Start = HM_Assassin->GetActorLocation();
	FVector End = HM_Assassin->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_Assassin);

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

					I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_AssaSecondAttack::BeginBeatedFunction);
					I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_AssaSecondAttack::EndBeatedFunction);

					// 1.1 Set Hit Attribute
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_Assassin->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

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

void UCHM_AssaSecondAttack::BeginBeatedFunction(AActor * Subject)
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

void UCHM_AssaSecondAttack::EndBeatedFunction(AActor * Subject)
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

