#include "CUpset_Burn.h"
#include "Global.h"

UCUpset_Burn::UCUpset_Burn()
{
	//Super
	State = EHitUpset::BURN;
}

void UCUpset_Burn::StartConditionOnActor(AActor * Owner)
{
	Super::StartConditionOnActor(Owner);
	check(Owner);
	
	//@Particle ON
	FVector OwnerLocation = Owner->GetActorLocation();
	BurnParticleComp->SetWorldLocation(OwnerLocation);
	
	BurnParticleComp->SetActive(true);
}

void UCUpset_Burn::UpdateConditionOnActor(AActor * Owner, float DeltaTime)
{
	Super::UpdateConditionOnActor(Owner, DeltaTime);

	//@Particle Position
	FVector OwnerLocation = Owner->GetActorLocation();
	BurnParticleComp->SetWorldLocation(OwnerLocation);
}

void UCUpset_Burn::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Burn* OverlapCondition = Cast< UCUpset_Burn>(OverlappedCondition);
	check(OverlapCondition);

	//@ÃÊ±âÈ­
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();

	//@Damage ÁßÃ¸
	float InputConditionDamage = OverlapCondition->GetSecondDamage();
	const float InputDamage = InputConditionDamage + OneSecondDamage;
	OverlapCondition->SetSecondDamage(InputDamage);

	CLog::Print(L"Burn Overlap!!");
}

void UCUpset_Burn::EndConditionOnActor(AActor * Owner)
{
	Super::EndConditionOnActor(Owner);
	check(Owner);

	//@Particle OFF
	BurnParticleComp->SetActive(false);
	BurnParticleComp = nullptr;
}
