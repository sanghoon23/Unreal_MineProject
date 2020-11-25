#include "CDamageType_Freeze.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Freeze.h"

UCDamageType_Freeze::UCDamageType_Freeze()
{
	// Super
	{
		TypeNumber = 7;
		DamageType = FDamageType::FREEZE;
	}

	// Attribute
	{
		bCausedByWorld = false;
		bScaleMomentumByMass = true;
		bRadialDamageVelChange = false;

		DamageImpulse = 100.0f;
		DestructibleImpulse = 100.0f;
		DestructibleDamageSpreadScale = 100.0f;
	}

	FString strPath = L"";

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_FreezeTypeFill.Tex_FreezeTypeFill'";
	ConstructorHelpers::FObjectFinder<UTexture2D> FreezeTexture(*strPath);
	if (FreezeTexture.Succeeded())
	{
		FreezeConditionUITexture = FreezeTexture.Object;
	}


	//@LOAD Freeze Particle - ParticleComp
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_FreezingActor.PS_FreezingActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> FreezePT(*strPath);
		if (FreezePT.Succeeded())
		{
			FreezeParticle = FreezePT.Object;
		}
	}

	//@LOAD 'Under' Freeze Particle - ParticleComp
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_UnderFreezingActor.PS_UnderFreezingActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> FreezeUnderPT(*strPath);
		if (FreezeUnderPT.Succeeded())
		{
			FreezeUnderParticle = FreezeUnderPT.Object;
		}
	}
}

void UCDamageType_Freeze::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@예외처리
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* PawnController = Cast<APawn>(Subject)->GetController();
	check(PawnController);

	IIC_Charactor* const I_Charactor = Cast<IIC_Charactor>(DamagedPawn);
	check(I_Charactor);

	//@Create ConditionData
	UCUpset_Freeze* FreezeConditionData = NewObject<UCUpset_Freeze>();
	check(FreezeConditionData);
	FreezeConditionData->SetCauser(Subject); //@주체자 설정
	FreezeConditionData->ApplyTime = GetFreezingTime();
	FreezeConditionData->SetDamageSubjectController(PawnController);

	/* Freeze Particle 은 UnderParticle 도 존재 */
	//@Warning - HitComp 에 Under까지 넣기 애매함 - (우선 Default 를 쓰자)
	FreezeConditionData->SetFreezeParticle(FreezeParticle);
	FreezeConditionData->SetFreezeUnderParticle(FreezeUnderParticle);

	//@DamagedActor 가 공격 당할 수 있다면,
	if (DamagedActorHitComp->IsDamagedFromOther() == true)
	{
		FDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = GetClass();
		FreezeConditionData->SetDamageEvent(DamageEvent);
		DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, Subject);
	}
	
	//@캐릭터가 죽었다면,
	IfTrueRet(I_Charactor->IsDeath());

	//@DamageTypeEffet 를 사용하지 않는다면, Damage 만, 들어간다.
	const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
	IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		FreezeConditionData->SetTextureUI(Texture);
	}

	//@Copy Delegate
	{
		FreezeConditionData->OnDelStartCondition = OnLinkStartUpsetCondition;
		FreezeConditionData->OnDelEndCondition = OnLinkEndUpsetCondition;
	}

	//@ADD
	bool bAddResult = DamagedActorHitComp->AddConditionData(FreezeConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp FREEZE AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Freeze::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
	SetFreezingTime(Data.FreezingTime);
}
