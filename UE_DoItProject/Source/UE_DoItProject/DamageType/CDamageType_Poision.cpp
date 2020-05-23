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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_PoisionWithTargetInfo.Test_UI_PoisionWithTargetInfo'";
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

	//@Create ConditionData
	UCUpset_Poision* PoisionConditionData = NewObject<UCUpset_Poision>();
	check(PoisionConditionData);
	PoisionConditionData->ApplyTime = GetPoisioningTime();
	PoisionConditionData->SetDamageSubjectController(PawnController);
	ACharacter* DamagedCharactor = Cast<ACharacter>(DamagedPawn);
	if (DamagedCharactor != nullptr)
	{
		PoisionConditionData->SetOriginMaterial(DamagedCharactor->GetMesh()->GetMaterial(0));
	}
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

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		PoisionConditionData->TextureUI = Texture;
	}

	//@Poision Material 변경
	if (DamagedCharactor != nullptr)
	{
		DamagedCharactor->GetMesh()->SetMaterial(0, DamagedActorHitComp->GetPoisionMaterialOrNull());
	}

	bool bAddResult = DamagedActorHitComp->AddConditionData(PoisionConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp BURN AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Poision::OnDamageDelegate(AActor * DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
