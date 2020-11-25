#include "CHM_MaoFourAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Monster/CHM_PengMao.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_AbilityComp.h"

#include "Actor/Figure/CSkillRangeDisplay.h"

UCHM_MaoFourAttack::UCHM_MaoFourAttack()
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
		UAnimMontage* Mao_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Attack/PengMaoAnim_FourAttack_Montage.PengMaoAnim_FourAttack_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Mao_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Mao_FirstAttack);
	}

#pragma endregion

	//@LOAD Particle - SlowerDamage
	{
		//@Attack
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/PengMao/P_HMMaoFourAttack.P_HMMaoFourAttack'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> AttackPT(*Path);
		if (AttackPT.Succeeded())
			P_AttackCloud = AttackPT.Object;

		//@Slower - Root
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Root.PS_Slower_FromFreeze_Root'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_1(*Path);
		if (SlowerPT_1.Succeeded())
			SlowerParticle_Root = SlowerPT_1.Object;

		//@Slower - Body
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Body.PS_Slower_FromFreeze_Body'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_2(*Path);
		if (SlowerPT_2.Succeeded())
			SlowerParticle_Body = SlowerPT_2.Object;
	}
}

void UCHM_MaoFourAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_PengMao);
	check(I_Charactor);

	//@Create SkillRangeDisplay
	{
		FTransform Transform = FTransform::Identity;
		FActorSpawnParameters Params;
		Params.Owner = HM_PengMao;
		for (int i = 0; i < 6; ++i)
		{
			ACSkillRangeDisplay* InsertActor = HM_PengMao->GetWorld()->SpawnActor<ACSkillRangeDisplay>
				(ACSkillRangeDisplay::StaticClass(), Transform, Params);

			check(InsertActor);
			if (InsertActor != nullptr) //@위치 지정
			{
				InsertActor->GetRootComponent()->AttachToComponent
				(
					HM_PengMao->GetMesh(),
					FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
					"root"
				);

				FVector OwnerLocation = HM_PengMao->GetActorLocation();
				OwnerLocation.Z = HM_PengMao->GetActorLocation().Z - HM_PengMao->GetDefaultHalfHeight() + SkillAttackRangeDisplayHeight;
				FVector OwnerForward = HM_PengMao->GetActorForwardVector();
				FRotator Rotate = FRotator(0.0f, 60.0f * i, 0.0f);
				FVector Dir = Rotate.RotateVector(OwnerForward);
				Dir.Normalize();
				FVector InsertLocation = OwnerLocation + (Dir * 700.0f);

				//@Bind - Delegate
				InsertActor->OnDelOverlapSkillRange.AddUObject
				(
					this, &UCHM_MaoFourAttack::DelSkillRangeAttackOtherPawn
				);

				//@Timer Particle
				FTransform ParticleTransform;
				ParticleTransform.SetLocation(FVector(0.0f, 0.0f, 400.0f));
				//FVector ParticleLocation = InsertLocation;
				//ParticleLocation.Z += 400.0f;
				//ParticleTransform.SetLocation(ParticleLocation);
				InsertActor->SetParticleSystem(P_AttackCloud, ParticleTransform);

				//@Set
				InsertActor->SetBackGroundDecalSize(6.0f);
				InsertActor->SetCollisionBoxExtent(FVector2D(80.0f));
				InsertActor->SetActorLocation(InsertLocation);
				InsertActor->SetVisibility(false);

				SkillRangeDisplayArray.Emplace(InsertActor);
			}
		}
	}
}

void UCHM_MaoFourAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UAnimInstance* OwnerAnimInst = HM_PengMao->GetMesh()->GetAnimInstance();
	if (OwnerAnimInst != nullptr && bAttacking == true) //@공격이 실행됐을 때,
	{
		float CurrentMonPos = OwnerAnimInst->Montage_GetPosition(AttackMontages[0]);
		float StartSectionLength, EndSectionLength = 0.0f;
		const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
		AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

		if (CurrentMonPos >= StartSectionLength) //@다음 섹션(NextAction)
		{
			OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], NextSectionPlayRate);
		}
	}
}

void UCHM_MaoFourAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_MaoFourAttack::BeginAttack(AActor * DoingActor)
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

		float StartSectionLength, EndSectionLength = 0.0f;
		const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
		AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

		//@Start Fill - SkillRange
		for (ACSkillRangeDisplay* Actor : SkillRangeDisplayArray)
		{
			Actor->SetVisibility(true);

			Actor->FillStart
			(
				HM_PengMao, 
				StartSectionLength * (1 / StartSectionPlayRate), //@섹션의 시작 지점
				(EndSectionLength - StartSectionLength) * (1 / NextSectionPlayRate) //@ 섹션의끝 - 섹션의시작 +Offset
			);
		}
	}
}

//@Warning - AttackOtherPawn 을 대신할 SkillRangeDisplay 에 Bind 될 함수.
void UCHM_MaoFourAttack::DelSkillRangeAttackOtherPawn(AActor * Subject)
{
	check(Subject);
	FVector ActorForward = Subject->GetActorForwardVector();
	FVector Start = Subject->GetActorLocation();

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
					//#1029_이미 맞은 액터라면,


					// 1.1 Set Hit Attribute
					//@맞은 애의 뒤쪽으로 HitDir
					FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - HM_PengMao->GetActorLocation();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					I_HitComp->SetHitDirection(HitDirection);
					//I_HitComp->SetHitMoveSpeed(0.3f);

					//@Set Delegate - OnKeyInputBlock 시키기 위해
					UCDamageType_Freeze* DT_Freeze = NewObject<UCDamageType_Freeze>();
					DT_Freeze->SetDamageImpulse(10.0f);
					DT_Freeze->SetFreezingTime(2.0f);
					//DT_Freeze->OnLinkStartUpsetCondition.RemoveAll(DT_Freeze);
					DT_Freeze->OnLinkStartUpsetCondition.AddUObject(this, &UCHM_MaoFourAttack::DelStartFreezeConditionType);

					//DT_Freeze->OnLinkEndUpsetCondition.RemoveAll(DT_Freeze);
					DT_Freeze->OnLinkEndUpsetCondition.AddUObject(this, &UCHM_MaoFourAttack::DelEndFreezeConditionType);

					I_HitComp->OnHit(HM_PengMao, DT_Freeze, DT_Freeze->DamageImpulse);
				}
				else
					UE_LOG(LogTemp, Warning, L"CHM_MaoFourAttack CallAttack - HitComp Null!!");

				//@느려지게 하기
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

					UCPLAbility_SpeedDown* AbilitySpeedDowner = NewObject<UCPLAbility_SpeedDown>();
					AbilitySpeedDowner->OnDelStartTimerAbility.AddLambda([PTComp_SlowerRoot, PTComp_SlowerBody](AActor*)
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

						AbilitySpeedDowner->SetAppliedActor(OverlapResult.GetActor());
						HitI_Charactor->GetIAbilityComp()->AddAbility(AbilitySpeedDowner);
					}
				}//(느려지게 하기)

			}//(Charactor)
			else
				UE_LOG(LogTemp, Warning, L"CHM_MaoFourAttack CallAttack - Charactor Null!!");
		}
	}
}

void UCHM_MaoFourAttack::DelStartFreezeConditionType(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanNotMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFourAttack StartBeatedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OnBlockAction();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFourAttack StartBeatedFunc, I_Player NULL!!");
}

void UCHM_MaoFourAttack::DelEndFreezeConditionType(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanMove();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFourAttack EndBeatedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OffBlockAction();
	}
	else UE_LOG(LogTemp, Warning, L"MaoFourAttack EndBeatedFunc, I_Player NULL!!");
}


