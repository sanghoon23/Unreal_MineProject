#include "CHM_MaoThirdAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Monster/CHM_PengMao.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_AbilityComp.h"

UCHM_MaoThirdAttack::UCHM_MaoThirdAttack()
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
		UAnimMontage* Mao_ThirdAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Attack/PengMaoAnim_ThirdAttack_Montage.PengMaoAnim_ThirdAttack_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MThirdAttack(*Path);
		if (MThirdAttack.Succeeded())
			Mao_ThirdAttack = MThirdAttack.Object;

		AttackMontages.Emplace(Mao_ThirdAttack);
	}

#pragma endregion

	////@LOAD Particle - SlowerDamage
	//{
	//	//@Root
	//	Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Root.PS_Slower_FromFreeze_Root'";
	//	ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_1(*Path);
	//	if (SlowerPT_1.Succeeded())
	//		SlowerParticle_Root = SlowerPT_1.Object;

	//	//@Body
	//	Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Body.PS_Slower_FromFreeze_Body'";
	//	ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_2(*Path);
	//	if (SlowerPT_2.Succeeded())
	//		SlowerParticle_Body = SlowerPT_2.Object;
	//}
}

void UCHM_MaoThirdAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_PengMao);
	check(I_Charactor);

#pragma region Create DamageType

	DT_AirFirstAttack = NewObject< UCDamageType_Air>();
	DT_AirFirstAttack->SetAirAttackHeight(400.0f);
	DT_AirFirstAttack->SetDamageImpulse(10.0f);

#pragma endregion

}

void UCHM_MaoThirdAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UAnimInstance* OwnerAnimInst = HM_PengMao->GetMesh()->GetAnimInstance();
	if (OwnerAnimInst != nullptr && bAttacking == true)
	{
		float CurrentMonPos = OwnerAnimInst->Montage_GetPosition(AttackMontages[0]);
		float StartSectionLength, EndSectionLength = 0.0f;
		const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
		AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

		if (CurrentMonPos >= StartSectionLength)
		{
			OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], NextSectionPlayRate);
		}
	}

	//@Before Code - 공격이 나오지 않고, 메모리에서 이 클래스가 삭제되는 현상
	//UAnimInstance* OwnerAnimInst = HM_PengMao->GetMesh()->GetAnimInstance();
	//if (OwnerAnimInst != nullptr && bCheckSectionRange == true)
	//{
	//	float CurrentMonPos = OwnerAnimInst->Montage_GetPosition(AttackMontages[0]);
	//	float StartSectionLength, EndSectionLength = 0.0f;
	//	const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
	//	AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

	//	if (CurrentMonPos >= StartSectionLength)
	//	{
	//		bCheckSectionRange = false;
	//		OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], NextSectionPlayRate);
	//	}
	//	else
	//	{
	//		float CurrentPlayRate = OwnerAnimInst->Montage_GetPlayRate(AttackMontages[0]);
	//		CurrentPlayRate -= 0.01f;
	//		FMath::Clamp(CurrentPlayRate, 0.1f, 1.0f);
	//		OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], CurrentPlayRate);
	//	}
	//}
}

void UCHM_MaoThirdAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_MaoThirdAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_PengMao->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(HM_PengMao->GetIEquipComp()->GetEquiping()); //@Equping Check
	//IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_PengMao->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			StartSectionPlayRate, false
		);

		// @공격 중 앞으로 이동.
		AttackMoveDir = HM_PengMao->GetActorForwardVector();
		AttackMoveSpeed = 2.0f;
	}
}

void UCHM_MaoThirdAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);

	FVector ActorForward = HM_PengMao->GetActorForwardVector();
	FVector Start = HM_PengMao->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_PengMao);

	TArray<FOverlapResult> OverlapResults;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults
		, Start
		, FQuat::Identity
		, I_Charactor->GetCharactorUsingAttackChannel() // @MonsterAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), Start, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			IIC_Charactor* HitI_Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
			if (HitI_Charactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* I_HitComp = HitI_Charactor->GetIHitComp();
				if (I_HitComp != nullptr)
				{
					//@Set Delegate - OnKeyInputBlock 시키기 위해
					I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_MaoThirdAttack::DelBeatedStartTarget);
					I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_MaoThirdAttack::DelBeatedEndTaraget);

					// 1.1 Set Hit Attribute
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - HM_PengMao->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

					I_HitComp->SetHitMoveSpeed(3.0f);
					I_HitComp->OnHit(HM_PengMao, DT_AirFirstAttack, DT_AirFirstAttack->DamageImpulse);
				}
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");

			}//(Charactor)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_MaoThirdAttack::DelBeatedStartTarget(AActor * Subject)
{
	IIC_Charactor* SubjectI_Charactor = Cast<IIC_Charactor>(Subject);
	if (SubjectI_Charactor != nullptr)
	{
		SubjectI_Charactor->CanNotMove();
	}

	IIC_Player* SubjectI_Player = Cast<IIC_Player>(Subject);
	if (SubjectI_Player != nullptr)
	{
		SubjectI_Player->OnBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoThridAttack BeginBetedFunc, I_Player NULL!!");
}

void UCHM_MaoThirdAttack::DelBeatedEndTaraget(AActor * Subject)
{
	IIC_Charactor* SubjectI_Charactor = Cast<IIC_Charactor>(Subject);
	if (SubjectI_Charactor != nullptr)
	{
		SubjectI_Charactor->CanMove();
	}

	IIC_Player* SubjectI_Player = Cast<IIC_Player>(Subject);
	if (SubjectI_Player != nullptr)
	{
		SubjectI_Player->OffBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"MaoThridAttack EndBetedFunc, I_Player NULL!!");
}

