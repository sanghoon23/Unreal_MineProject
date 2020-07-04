#include "CDamageType_Slower.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Slower.h"

UCDamageType_Slower::UCDamageType_Slower()
{
	// Super
	{
		TypeNumber = 8;
		DamageType = FDamageType::SLOWER;
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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Origin/Tex_SpeedType.Tex_SpeedType'";
	ConstructorHelpers::FObjectFinder<UTexture2D> SlowerTexture(*strPath);
	if (SlowerTexture.Succeeded())
	{
		SlowerConditionUITexture = SlowerTexture.Object;
	}
}

void UCDamageType_Slower::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* const PawnController = DamagedPawn->GetController();
	check(PawnController);

	//@예외처리 - Damage 를 받지 않는 상황
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	//@Create ConditionData
	UCUpset_Slower* SlowerConditionData = NewObject<UCUpset_Slower>();
	check(SlowerConditionData);
	SlowerConditionData->ApplyTime = GetSlowerTime();
	SlowerConditionData->SetDamageSubjectController(PawnController);
	SlowerConditionData->SetSlowerParticle(SlowerParticlesMap);

	//@Damage Class
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	SlowerConditionData->SetDamageEvent(DamageEvent);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	if (I_Charactor != nullptr)
	{
		//@죽음 확인
		IfTrueRet(I_Charactor->IsDeath() == true);

		//@느려지는 Ability 추가.
		AbilitySlower->SetAppliedActor(DamagedActor); //@액터지정해주어야 함. - AbilityComp Add 안에
		I_Charactor->GetIAbilityComp()->AddAbility(AbilitySlower);
	}

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		SlowerConditionData->SetTextureUI(Texture);
	}

	bool bAddResult = DamagedActorHitComp->AddConditionData(SlowerConditionData);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp SLOWER AddConditionData Derived NULL!!");
	}
}

void UCDamageType_Slower::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}

void UCDamageType_Slower::SetAbilityForSlower(UCBaseAbility * Ability)
{
	check(Ability);
	AbilitySlower = Ability;
}

void UCDamageType_Slower::AddSlowerParticle(EAttachPointType Type, UParticleSystem * PT)
{
	check(PT);
	SlowerParticlesMap.Add(Type, PT);
}
