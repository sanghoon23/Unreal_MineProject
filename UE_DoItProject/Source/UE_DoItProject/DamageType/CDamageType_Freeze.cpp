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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_FreezeWithTargetInfo.Test_UI_FreezeWithTargetInfo'";
	ConstructorHelpers::FObjectFinder<UTexture2D> FreezeTexture(*strPath);
	if (FreezeTexture.Succeeded())
	{
		FreezeConditionUITexture = FreezeTexture.Object;
	}
}

void UCDamageType_Freeze::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* const PawnController = DamagedPawn->GetController();
	check(PawnController);

	//@Create ConditionData
	UCUpset_Freeze* FreezeConditionData = NewObject<UCUpset_Freeze>();
	check(FreezeConditionData);
	FreezeConditionData->ApplyTime = GetFreezingTime();
	FreezeConditionData->SetDamageSubjectController(PawnController);
	UParticleSystemComponent* FreezeParticleComp = DamagedActorHitComp->GetFreezeParticleCompOrNull();
	if (FreezeParticleComp != nullptr)
	{
		FreezeConditionData->SetFreezeParticleComp(FreezeParticleComp);
	}

	//@Damage Class
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	FreezeConditionData->SetDamageEvent(DamageEvent);

	//@TakeDamage
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);

	//@Á×À½ È®ÀÎ
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	if (I_Charactor != nullptr)
	{
		IfTrueRet(I_Charactor->IsDeath() == true);
	}

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		FreezeConditionData->TextureUI = Texture;
	}

	bool bAddResult = DamagedActorHitComp->AddConditionData(FreezeConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp FREEZE AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Freeze::OnDamageDelegate(AActor * DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
