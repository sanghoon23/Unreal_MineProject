#include "CUpset_Poision.h"
#include "Global.h"

#include "GameFramework/Character.h"

UCUpset_Poision::UCUpset_Poision()
{
	//Super
	State = EHitUpset::POISION;
}

void UCUpset_Poision::UpdateCondition(APawn * Owner, float DeltaTime)
{
	Super::UpdateCondition(Owner, DeltaTime);
	check(Owner);

	SecondTimer += DeltaTime;
	if (SecondTimer > DamagedTime)
	{
		SecondTimer = 0.0f;

		//@Take Damage
		Owner->TakeDamage(SecondDamage, DamageEvent, GetDamageSubjectController(), Owner);
	}
}

void UCUpset_Poision::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	ACharacter* Charactor = Cast<ACharacter>(Owner);
	if (Charactor != nullptr)
	{
		//#Edit 0509 -
		//@���� Material �� ��ȯ
		check(OriginMaterial);
		if (OriginMaterial != nullptr)
		{
			Charactor->GetMesh()->SetMaterial(0, OriginMaterial);
		}
	}
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
	const float InputConditionDamage = OverlapCondition->GetSecondDamage();
	SecondDamage += InputConditionDamage;

	CLog::Print(L"Poision Overlap!!");
	CLog::Print(SecondDamage);
}

void UCUpset_Poision::SetOriginMaterial(UMaterialInterface * Mat)
{
	//#Edit 0509 -
	//@�켱�� Material ���� check �����ν� Material �� ������ ��ȿ�� ���̶� ����
	check(Mat);
	if (Mat != nullptr)
	{
		OriginMaterial = Mat;
	}
}
