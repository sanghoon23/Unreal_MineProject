#include "CUpset_Poision.h"
#include "Global.h"

#include "GameFramework/Character.h"

UCUpset_Poision::UCUpset_Poision()
{
	//Super
	State = EHitUpset::POISION;
}

void UCUpset_Poision::UpdateConditionOnActor(AActor * Owner, float DeltaTime)
{
	Super::UpdateConditionOnActor(Owner, DeltaTime);
}

void UCUpset_Poision::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Poision* OverlapCondition = Cast<UCUpset_Poision>(OverlappedCondition);
	check(OverlapCondition);

	//@�ʱ�ȭ
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();

	//@Damage ��ø
	float InputConditionDamage = OverlapCondition->GetSecondDamage();
	const float InputDamage = InputConditionDamage + OneSecondDamage;
	OverlapCondition->SetSecondDamage(InputDamage);

	CLog::Print(L"Poision Overlap!!");
}

void UCUpset_Poision::EndConditionOnActor(AActor * Owner)
{
	Super::EndConditionOnActor(Owner);
	check(Owner);

	ACharacter* Charactor = Cast<ACharacter>(Owner);
	if (Charactor != nullptr)
	{
		//@���� Material �� ��ȯ
		check(OriginMaterial);
		Charactor->GetMesh()->SetMaterial(0, OriginMaterial);
	}
}
