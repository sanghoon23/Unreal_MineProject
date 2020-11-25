#include "CHM_MaoSecondAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Monster/CHM_PengMao.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_AbilityComp.h"

UCHM_MaoSecondAttack::UCHM_MaoSecondAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<uint8>(EHM_MaoSecondComboType::NONE);
		MaxComboNum = static_cast<uint8>(EHM_MaoSecondComboType::END);
	}

	AttackRangeVec.Init(0.0f, static_cast<uint8>(EHM_MaoSecondComboType::END));
	AttackRadiusVec.Init(0.0f, static_cast<uint8>(EHM_MaoSecondComboType::END));

	FString Path = L"";

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Mao_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Attack/PenMaoAnim_SecondAttack_Montage.PenMaoAnim_SecondAttack_Montage'";
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
}

void UCHM_MaoSecondAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_PengMao);
	check(I_Charactor);

#pragma region Create DamageType

	//DT_Freeze = NewObject<UCDamageType_Freeze>();
	//DT_Freeze->SetDamageImpulse(10.0f);
	//DT_Freeze->SetFreezingTime(2.0f);

#pragma endregion

	//@Create Ability
	{
		AbilitySpeedDowner = NewObject<UCPLAbility_SpeedDown>();
	}

	//@Setting Value
	{
		OffSetAttackRangeForStart = 50.0f; //@Range - 100.0f

		//@공격 거리(Sphere 가 움직일 범위)
		AttackRangeVec[1] = 150.0f;
		AttackRangeVec[2] = 200.0f;

		//@공격 둘레 - Sphere
		AttackRadiusVec[1] = 200.0f;
		AttackRadiusVec[2] = 300.0f;
	}

}

void UCHM_MaoSecondAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_MaoSecondAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_MaoSecondAttack::BeginAttack(AActor * DoingActor)
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
			0.9f, false
		);
	}
}

bool UCHM_MaoSecondAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_MaoSecondComboType::COMBO_TWO))
		return true;

	return false;
}

/*
#Edit 0701 - 
부모에서 EndAttack 시 CurrentComboNum 을 0 으로 맞춰주기 때문에, 
Enum - NONE 을 포함시켜서 콤보 인덱스를 맞추어놨다.
ex) 첫번째 공격 1, 두번째 공격 2...
*/
void UCHM_MaoSecondAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

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
			IIC_Charactor* HitI_Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
			if (HitI_Charactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* I_HitComp = HitI_Charactor->GetIHitComp();
				if (I_HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_PengMao->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);

					I_HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());
					I_HitComp->OnHit(HM_PengMao, DamageType, DamageType->DamageImpulse);

					if ((IsLastCombo() == true)) //마지막 일격
					{
						//@Set Delegate - OnKeyInputBlock 시키기 위해
						//I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_MaoSecondAttack::BeginBeatedFunction);
						I_HitComp->EndBeatedFunc.AddUObject(this, &UCHM_MaoSecondAttack::EndBeatedFunction);

						//#1030 -
						/*
						Freeze 시 OnBlockAction,
						EndBeated 에 OffBlockAction
						*/
						//@Set Timer - Freeze Timer
						UCDamageType_Freeze* DT_Freeze = NewObject<UCDamageType_Freeze>();
						DT_Freeze->SetDamageImpulse(10.0f);
						DT_Freeze->SetFreezingTime(2.0f);

						DT_Freeze->OnLinkStartUpsetCondition.AddUObject(this, &UCHM_MaoSecondAttack::BeginBeatedFunction);
						//DT_Freeze->OnLinkEndUpsetCondition.AddUObject(this, &UCHM_MaoSecondAttack::EndBeatedFunction);

						FTimerHandle FreezeTimerHandle;
						FreezeTimerDelegate = FTimerDelegate::CreateUObject(this, &UCHM_MaoSecondAttack::TimerFreezeHittedActor, HitResult.GetActor(), DT_Freeze);
						(HitResult.GetActor())->GetWorldTimerManager().SetTimer
						(
							FreezeTimerHandle,
							FreezeTimerDelegate,
							1.0f,
							false
						);
					}
					else //첫번째 공격
					{
						//@Slower 느려지게 하기.
						{
							IIC_MeshParticle* I_MeshParticle = HitI_Charactor->GetIMeshParticle();
							check(I_MeshParticle);

							FTransform RootTrans = FTransform::Identity;
							RootTrans.SetScale3D(FVector(2.0f));
							UParticleSystemComponent* PTComp_SlowerRoot = I_MeshParticle->SpawnParticleAtMesh
							(
								SlowerParticle_Root,
								EAttachPointType::ROOT,
								EAttachPointRelative::NONE,
								EAttachLocation::SnapToTarget,
								RootTrans
							);

							FTransform BodyTrans = FTransform::Identity;
							BodyTrans.SetScale3D(FVector(2.0f));
							UParticleSystemComponent* PTComp_SlowerBody = I_MeshParticle->SpawnParticleAtMesh
							(
								SlowerParticle_Body,
								EAttachPointType::ROOT,
								EAttachPointRelative::NONE,
								EAttachLocation::SnapToTarget,
								BodyTrans
							);

							AbilitySpeedDowner->OnEndTimerAbility.AddLambda([PTComp_SlowerRoot, PTComp_SlowerBody](AActor*)
							{
								PTComp_SlowerRoot->SetActive(false);
								PTComp_SlowerBody->SetActive(false);
							});

							//@Ability Insert - 부정적 효과 넣기
							IIC_AbilityComp* HitI_AbilityComp = HitI_Charactor->GetIAbilityComp();
							if (HitI_AbilityComp != nullptr && (IsLastCombo() == false))
							{
								FAbilityValue InputValue;
								InputValue.Sort = EAbilitySort::SAVEARRAY;
								InputValue.bTimer = true;
								InputValue.Timer = 7.0f;
								InputValue.Value = AbilityDownSpeedValue;
								AbilitySpeedDowner->SetAbilityValue(InputValue);

								AbilitySpeedDowner->SetAppliedActor(HitResult.GetActor());
								HitI_Charactor->GetIAbilityComp()->AddAbility(AbilitySpeedDowner);
							}
						}
					}//else(첫번째 공격)
				}
				else UE_LOG(LogTemp, Warning, L"CHM_MaoSecondAttack CallAttack - HitComp Null!!");

			}//(Charactor)
			else UE_LOG(LogTemp, Warning, L"CHM_MaoSecondAttack CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_MaoSecondAttack::BeginBeatedFunction(AActor * Subject)
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
		SubjectPlayerInterface->OnBlockAction();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack BeginBetedFunc, I_Player NULL!!")
}

void UCHM_MaoSecondAttack::EndBeatedFunction(AActor * Subject)
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
		SubjectPlayerInterface->OffBlockAction();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFirstAttack EndBetedFunc, I_Player NULL!!");
}

void UCHM_MaoSecondAttack::TimerFreezeHittedActor(AActor * Subject, UCDamageType_Freeze* DT_Freeze)
{
	IIC_Charactor* SubjectI_Charactor = Cast<IIC_Charactor>(Subject);
	if (SubjectI_Charactor != nullptr)
	{
		IIC_HitComp* SubjectI_HitComp = SubjectI_Charactor->GetIHitComp();
		if (SubjectI_HitComp != nullptr)
		{
			SubjectI_HitComp->OnHit(HM_PengMao, DT_Freeze, DT_Freeze->DamageImpulse);
		}
	}
}

