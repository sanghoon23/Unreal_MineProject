#include "CUpset_Freeze.h"
#include "Global.h"

UCUpset_Freeze::UCUpset_Freeze()
{
	//Super
	State = EHitUpset::FREEZE;
}

void UCUpset_Freeze::StartConditionOnActor(AActor * Owner)
{
	Super::StartConditionOnActor(Owner);
	check(Owner);

	//@Particle ON
	FVector OwnerLocation = Owner->GetActorLocation();
	FreezeParticleComp->SetWorldLocation(OwnerLocation);

	FreezeParticleComp->SetActive(true);
}

void UCUpset_Freeze::UpdateConditionOnActor(AActor * Owner, float DeltaTime)
{
	Super::UpdateConditionOnActor(Owner, DeltaTime);

	//@Particle Position
	FVector OwnerLocation = Owner->GetActorLocation();
	FreezeParticleComp->SetWorldLocation(OwnerLocation);
}

void UCUpset_Freeze::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Freeze* OverlapCondition = Cast<UCUpset_Freeze>(OverlappedCondition);
	check(OverlapCondition);

	//@ÃÊ±âÈ­
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();

	CLog::Print(L"Freeze Overlap!!");
}

void UCUpset_Freeze::EndConditionOnActor(AActor * Owner)
{
	Super::EndConditionOnActor(Owner);
	check(Owner);

	//@Particle OFF
	FreezeParticleComp->SetActive(false);
	FreezeParticleComp = nullptr;
}
