#include "CUpset_Slower.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_MeshParticle.h"

UCUpset_Slower::UCUpset_Slower()
{
	//Super
	State = EHitUpset::SLOWER;
}

void UCUpset_Slower::StartCondition(APawn * Owner)
{
	Super::StartCondition(Owner);
	check(Owner);

	CLog::Print(L"Slower StartCondition!!");

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			for (auto& Value : SlowerParticlesMap)
			{
				const EAttachPointType Type = Value.Key;
				UParticleSystem* const Particle = Value.Value;
				check(Particle);

				UParticleSystemComponent* PTComp = I_MeshParticle->SpawnParticleAtMesh
				(
					Particle,
					Type,
					EAttachPointRelative::RELATIVE,
					EAttachLocation::SnapToTarget
				);

				if (PTComp == nullptr)
					UE_LOG(LogTemp, Warning, L"CUpset_Burn BurnParticleComp NULL!!");

				PTCompContainerForEnd.Emplace(PTComp);
			}

		}
	}

	//IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	//if (I_Charactor != nullptr)
	//{
	//	IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
	//	if (I_MeshParticle != nullptr)
	//	{
	//		check(BurnParticle);
	//		BurnParticleComp = I_MeshParticle->SpawnParticleAtMesh
	//		(
	//			BurnParticle,
	//			EAttachPointType::BODY,
	//			EAttachPointRelative::RELATIVE,
	//			EAttachLocation::SnapToTarget
	//		);

	//		if (BurnParticleComp == nullptr)
	//			UE_LOG(LogTemp, Warning, L"CUpset_Burn BurnParticleComp NULL!!");
	//	}
	//}//(I_Charactor != nullptr)
}

void UCUpset_Slower::UpdateCondition(APawn * Owner, float DeltaTime)
{
	Super::UpdateCondition(Owner, DeltaTime);
	check(Owner);
}

void UCUpset_Slower::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	for (UParticleSystemComponent* PTComp : PTCompContainerForEnd)
	{
		PTComp->SetActive(false);
	}
	PTCompContainerForEnd.Empty();
}

void UCUpset_Slower::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Slower* OverlapCondition = Cast<UCUpset_Slower>(OverlappedCondition);
	check(OverlapCondition);

	//@ÃÊ±âÈ­
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();
	AController* InputController = OverlapCondition->GetDamageSubjectController();
	if (GetDamageSubjectController() != InputController)
	{
		SetDamageSubjectController(InputController);
	}
}

void UCUpset_Slower::SetSlowerParticle(TMap<EAttachPointType, class UParticleSystem*>& In)
{
	for (auto& Value : In)
	{
		SlowerParticlesMap.Add(Value.Key, Value.Value);
	}
}

