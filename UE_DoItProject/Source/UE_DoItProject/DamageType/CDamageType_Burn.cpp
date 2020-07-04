#include "CDamageType_Burn.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Burn.h"

UCDamageType_Burn::UCDamageType_Burn()
{
	// Super
	{
		TypeNumber = 5;
		DamageType = FDamageType::BURN;
	}

	// Attribute
	{
		bCausedByWorld = false;
		bScaleMomentumByMass = true;
		bRadialDamageVelChange = false;

		DamageImpulse = 5.0f;
		DestructibleImpulse = 0.0f;
		DestructibleDamageSpreadScale = 100.0f;
	}

	FString strPath = L"";

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_BurnTypeFill.Tex_BurnTypeFill'";
	ConstructorHelpers::FObjectFinder<UTexture2D> BurnTexture(*strPath);
	if (BurnTexture.Succeeded())
	{
		BurnConditionUITexture = BurnTexture.Object;
	}

	//@LOAD Burn Particle - ParticleComp
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_BurningActor.PS_BurningActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> BurnPT(*strPath);
		if (BurnPT.Succeeded())
		{
			BurnParticle = BurnPT.Object;
		}
	}
}

void UCDamageType_Burn::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* const PawnController = DamagedPawn->GetController();
	check(PawnController);

	//@예외처리 - Damage 를 받지 않는 상황
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	//@Create ConditionData
	UCUpset_Burn* BurnConditionData = NewObject<UCUpset_Burn>();
	check(BurnConditionData);
	BurnConditionData->ApplyTime = GetBurnTime();
	BurnConditionData->SetDamageSubjectController(PawnController);
	UParticleSystem* HitCompBurnParticle = DamagedActorHitComp->GetBurnParticleOrNull();
	if (HitCompBurnParticle != nullptr)
	{
		BurnConditionData->SetBurnParticle(HitCompBurnParticle);
	}
	else //@Default
	{
		BurnConditionData->SetBurnParticle(BurnParticle);
	}

	BurnConditionData->SetSecondDamage(GetSecondDamageValue());

	//@Damage Class
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	BurnConditionData->SetDamageEvent(DamageEvent);

	//@TakeDamage
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);

	//@죽음 확인
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	if (I_Charactor != nullptr)
	{
		IfTrueRet(I_Charactor->IsDeath() == true);
	}

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		BurnConditionData->SetTextureUI(Texture);
	}

	bool bAddResult = DamagedActorHitComp->AddConditionData(BurnConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp BURN AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Burn::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}

