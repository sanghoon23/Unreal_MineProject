#include "CHM_MaoFirstAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_PengMao.h"

UCHM_MaoFirstAttack::UCHM_MaoFirstAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	AttackRangeVec.Init(0.0f, static_cast<uint8>(EHM_MaoFirstComboType::END));
	AttackRadiusVec.Init(0.0f, static_cast<uint8>(EHM_MaoFirstComboType::END));

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Mao_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Attack/PenMaoAnim_FirstAttack_Montage.PenMaoAnim_FirstAttack_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Mao_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Mao_FirstAttack);
	}

#pragma endregion

	//@LOAD Particle - SlowerDamage
	{
		//@Root
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Root.PS_Slower_FromFreeze_Root'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_1(*Path);
		if (SlowerPT_1.Succeeded())
			SlowerParticle_Root = SlowerPT_1.Object;

		//@Body
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Body.PS_Slower_FromFreeze_Body'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_2(*Path);
		if (SlowerPT_2.Succeeded())
			SlowerParticle_Body = SlowerPT_2.Object;
	}

	//@Create Ability
	{
		AbilitySpeedDowner = NewObject<UCPLAbility_SpeedDown>();
	}

}

void UCHM_MaoFirstAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_PengMao);
	check(I_Charactor);

#pragma region Create DamageType

	DT_Noraml = NewObject<UCDamageType_Normal>();
	DT_Noraml->SetDamageImpulse(10.0f);

	DT_Strong = NewObject<UCDamageType_StrongAttack>();
	DT_Strong->SetDamageImpulse(20.0f);

#pragma endregion

	//@Set Delegate - Ability
	{
		//@Start
		AbilitySpeedDowner->OnDelStartTimerAbility.AddLambda([&](AActor* AppliedActor)
		{
			IIC_Charactor* ApplyI_Charactor = Cast<IIC_Charactor>(AppliedActor);
			if (ApplyI_Charactor != nullptr)
			{
				//@파티클 실행
				IIC_MeshParticle* I_MeshParticle = ApplyI_Charactor->GetIMeshParticle();
				check(I_MeshParticle);

				FTransform RootTrans = FTransform::Identity;
				RootTrans.SetScale3D(FVector(2.0f));
				SlowerParticleComp_Root = I_MeshParticle->SpawnParticleAtMesh
				(
					SlowerParticle_Root,
					EAttachPointType::ROOT,
					EAttachPointRelative::NONE,
					EAttachLocation::SnapToTarget,
					RootTrans
				);

				FTransform BodyTrans = FTransform::Identity;
				BodyTrans.SetScale3D(FVector(2.0f));
				SlowerParticleComp_Body = I_MeshParticle->SpawnParticleAtMesh
				(
					SlowerParticle_Body,
					EAttachPointType::ROOT,
					EAttachPointRelative::NONE,
					EAttachLocation::SnapToTarget,
					BodyTrans
				);
			}
		});

		//@End
		AbilitySpeedDowner->OnEndTimerAbility.AddLambda([&](AActor* AppliedActor)
		{
			//@Particle OFF
			if (SlowerParticleComp_Root != nullptr)
				SlowerParticleComp_Root->SetActive(false);
			else CLog::Print(L"LHand Particle NULL!!");

			if (SlowerParticleComp_Body != nullptr)
				SlowerParticleComp_Body->SetActive(false);
			else CLog::Print(L"RHand Particle NULL!!");
		});
	}

	//@Setting Value
	{
		OffSetAttackRangeForStart = 100.0f; //@Range - 100.0f

		//@공격 거리(Sphere 가 움직일 범위)
		AttackRangeVec[1] = 200.0f;
		AttackRangeVec[2] = 200.0f;
		AttackRangeVec[3] = 200.0f;
		AttackRangeVec[4] = 200.0f;

		//@공격 둘레 - Sphere
		AttackRadiusVec[1] = 200.0f;
		AttackRadiusVec[2] = 200.0f;
		AttackRadiusVec[3] = 200.0f;
		AttackRadiusVec[4] = 300.0f;
	}
}

void UCHM_MaoFirstAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_MaoFirstAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_MaoFirstAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @IF TRUE RETURN
	IfTrueRet(HM_PengMao->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(HM_PengMao->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		HM_PengMao->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}

bool UCHM_MaoFirstAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_MaoFirstComboType::COMBO_FOUR))
		return true;

	return false;
}

/*
#Edit 0701 -
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에,
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_MaoFirstAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	//@현재 콤보 늘려줌
	++CurrentComboNum;
	const uint8 ComboNum = static_cast<uint8>(CurrentComboNum);

	FVector ActorForward = HM_PengMao->GetActorForwardVector();
	FVector Start = HM_PengMao->GetActorLocation() + ActorForward * OffSetAttackRangeForStart;
	FVector End = HM_PengMao->GetActorLocation() + ActorForward * AttackRangeVec[ComboNum];

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadiusVec[ComboNum]);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_PengMao);

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

	DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

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
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_PengMao->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

					//@DT_Slower
					//I_HitComp->OnHit(HM_PengMao, DT_Slower, 0.0f);

					if ((IsLastCombo() == true))
					{
						//@Set Delegate - 마무리 일격만 맞고 KeyInput 을 받지 않음.
						I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_MaoFirstAttack::BeginBeatedFunction);
						I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_MaoFirstAttack::EndBeatedFunction);

						//@DT_Strong
						I_HitComp->SetHitMoveSpeed(3.0f);
						I_HitComp->OnHit(HM_PengMao, DT_Strong, DT_Strong->DamageImpulse);
					}
					else
					{
						//@DT_Normal
						I_HitComp->SetHitMoveSpeed(1.5f);
						I_HitComp->OnHit(HM_PengMao, DT_Noraml, DT_Noraml->DamageImpulse);
					}
				}
				else
					UE_LOG(LogTemp, Warning, L"MaoFirstAttack CallAttack - HitComp Null!!");

				//@Ability Insert
				IIC_AbilityComp* I_AbilityComp = HitIneterfaceCharactor->GetIAbilityComp();
				if (I_AbilityComp != nullptr)
				{
					//CLog::Print(L"AbilityComp Not NULL!!");
					FAbilityValue InputValue;
					InputValue.bTimer = true;
					InputValue.Timer = 3.0f;
					InputValue.Value = AbilityDownSpeedValue;
					AbilitySpeedDowner->SetAbilityValue(InputValue);

					AbilitySpeedDowner->SetAppliedActor(HitResult.GetActor());
					HitIneterfaceCharactor->GetIAbilityComp()->AddAbility(AbilitySpeedDowner);
				}

			}//(Charactor)
			else
				UE_LOG(LogTemp, Warning, L"MaoFirstAttack CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_MaoFirstAttack::BeginBeatedFunction(AActor * Subject)
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
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Player NULL!!");
}

void UCHM_MaoFirstAttack::EndBeatedFunction(AActor * Subject)
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

void UCHM_MaoFirstAttack::BeginAbilityFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		//@파티클 실행
		IIC_MeshParticle* I_MeshParticle = SubjectCharactorInterface->GetIMeshParticle();
		check(I_MeshParticle);

		SlowerParticleComp_Root = I_MeshParticle->SpawnParticleAtMesh
		(
			SlowerParticle_Root,
			EAttachPointType::ROOT,
			EAttachPointRelative::NONE,
			EAttachLocation::SnapToTarget
		);

		SlowerParticleComp_Body = I_MeshParticle->SpawnParticleAtMesh
		(
			SlowerParticle_Body,
			EAttachPointType::ROOT,
			EAttachPointRelative::NONE,
			EAttachLocation::SnapToTarget
		);
	}
}

void UCHM_MaoFirstAttack::EndAbilityFunction(AActor * Subject)
{
	//@Particle OFF
	if (SlowerParticleComp_Root != nullptr)
		SlowerParticleComp_Root->SetActive(false);
	else CLog::Print(L"LHand Particle NULL!!");

	if (SlowerParticleComp_Body != nullptr)
		SlowerParticleComp_Body->SetActive(false);
	else CLog::Print(L"RHand Particle NULL!!");
}
