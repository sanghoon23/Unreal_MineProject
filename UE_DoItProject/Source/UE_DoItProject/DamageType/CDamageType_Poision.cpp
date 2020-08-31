#include "CDamageType_Poision.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Poision.h"

UCDamageType_Poision::UCDamageType_Poision()
{
	// Super
	{
		TypeNumber = 6;
		DamageType = FDamageType::POISION;
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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_PoisionsFilling.Tex_PoisionsFilling'";
	ConstructorHelpers::FObjectFinder<UTexture2D> PoisionTexture(*strPath);
	if (PoisionTexture.Succeeded())
	{
		PoisionConditionUITexture = PoisionTexture.Object;
	}
}

void UCDamageType_Poision::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* const PawnController = DamagedPawn->GetController();
	check(PawnController);

	//@예외처리
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	//@Create ConditionData
	UCUpset_Poision* PoisionConditionData = NewObject<UCUpset_Poision>();
	check(PoisionConditionData);
	PoisionConditionData->ApplyTime = GetPoisioningTime();
	PoisionConditionData->SetDamageSubjectController(PawnController);
	PoisionConditionData->SetSecondDamage(GetSecondDamageValue());

	//@Damage Class
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	PoisionConditionData->SetDamageEvent(DamageEvent);

	//@TakeDamage
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);

	//@죽음 확인
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	if (I_Charactor != nullptr)
	{
		IfTrueRet(I_Charactor->IsDeath() == true);
	}

	//@DamageTypeEffet 를 사용하지 않는다면, Damage 만, 들어간다.
	const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
	IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		PoisionConditionData->SetTextureUI(Texture);
	}

	//@Copy Delegate
	{
		PoisionConditionData->OnDelStartCondition = OnLinkStartUpsetCondition;
		PoisionConditionData->OnDelEndCondition = OnLinkEndUpsetCondition;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TMap<int32, class UMaterialInterface*> ChangeMaterialMap;
	DamagedActorHitComp->GetPoisionMaterialMaps(ChangeMaterialMap);
	if (ChangeMaterialMap.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, L"CDamageType_Poision Map NULL!! And Not ADD Condition");
	}
	else
	{
		ACharacter* DamagedCharactor = Cast<ACharacter>(DamagedPawn);
		check(DamagedCharactor);
		USkeletalMeshComponent* const SkeletalMesh = DamagedCharactor->GetMesh();
		check(SkeletalMesh);
		for (auto& Material : ChangeMaterialMap)
		{
			int Num = Material.Key;
			UMaterialInterface* GetOrigin = SkeletalMesh->GetMaterial(Num);

			DamagedCharactor->GetMesh()->SetMaterial(Num, Material.Value); //@Poision Material 변경
		}
		TMap<int32, class UMaterialInterface*> OriginMaterialMap;
		DamagedActorHitComp->GetOriginPoisionMaterialMaps(OriginMaterialMap);
		PoisionConditionData->SetOriginMaterial(OriginMaterialMap); //@Origin 적용
	}//else

	bool bAddResult = DamagedActorHitComp->AddConditionData(PoisionConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"CDamageType_Poision AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Poision::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
	SetPoisioningTime(Data.PoisionTime);
	SetSecondDamageValue(Data.PoisionSecondDamageValue);
}
