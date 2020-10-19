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
		CurrentComboNum = static_cast<uint8>(EHM_AssaFourComboType::NONE);
		MaxComboNum = static_cast<uint8>(EHM_AssaFourComboType::COMBO_SIX);
	}

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Assa_FourAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Mon_FourAttack01.HM_Assassin_Mon_FourAttack01'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> Mon_FourAttack(*Path);
		if (Mon_FourAttack.Succeeded())
			Assa_FourAttack = Mon_FourAttack.Object;

		AttackMontages.Emplace(Assa_FourAttack);
	}

	////@LOAD Particle
	//{
	//	Path = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/Assassin_FourAttackCasting.Assassin_FourAttackCasting'";
	//	ConstructorHelpers::FObjectFinder<UParticleSystem> P_LoadCast(*Path);
	//	if (P_LoadCast.Succeeded())
	//	{
	//		P_AttackCasting = P_LoadCast.Object;
	//	}
	//}

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

	//@Get StartSectionLength, EndSectionLength -->> 느리게 시작된 몽타주 다시 원래 속도로.
	const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
	AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

	CLog::Print(StartSectionLength);
	CLog::Print(EndSectionLength);
}

void UCHM_AssaFourAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CLog::Print(L"FourAttack Tick IN!!");

	UAnimInstance* OwnerAnimInst = HM_Assassin->GetMesh()->GetAnimInstance();
	if (OwnerAnimInst != nullptr && bAttacking == true && bSettingPlayRate == false) //@공격이 실행됐을 때,
	{
		float CurrentMonPos = OwnerAnimInst->Montage_GetPosition(AttackMontages[0]);
		CLog::Print(CurrentMonPos);
		if (CurrentMonPos >= StartSectionLength) //@다음 섹션(NextAction)
		{
			bSettingPlayRate = true;
			OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], NextSectionPlayRate);
		}
	}
}

void UCHM_AssaFourAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaFourAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	//@IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.2f, true
		);

		bSettingPlayRate = false;
	}
}

bool UCHM_AssaFourAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_AssaFourComboType::COMBO_SIX))
		return true;

	return false;
}

/*
#Edit 0701 -
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_AssaFourAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

	//@현재 콤보 늘려줌
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
	bool bHit = GetWorld()->SweepMultiByChannel
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

					I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_AssaFourAttack::BeginBeatedFunction);
					I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_AssaFourAttack::EndBeatedFunction);

					// 1.1 Set Hit Attributellll
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_Assassin->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

					I_HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());
					I_HitComp->OnHit(HM_Assassin, DamageType, DamageType->DamageImpulse);
				}
				else
					UE_LOG(LogTemp, Warning, L"AssaFourAttack CallAttack - HitComp Null!!");

			}//(Charactor)
			else
				UE_LOG(LogTemp, Warning, L"AssaFourAttack CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_AssaFourAttack::BeginBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanNotMove();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFourAttack BeginBetedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OnBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFourAttack BeginBetedFunc, I_Player NULL!!")
}

void UCHM_AssaFourAttack::EndBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanMove();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFourAttack EndBeatedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OffBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFourAttack EndBetedFunc, I_Player NULL!!");
}
