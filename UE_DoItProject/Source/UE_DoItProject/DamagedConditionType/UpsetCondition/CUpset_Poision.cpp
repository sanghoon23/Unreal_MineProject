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
		//@원래 Material 로 전환
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

	//@초기화
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();

	//@Damage 중첩
	const float InputConditionDamage = OverlapCondition->GetSecondDamage();
	SecondDamage += InputConditionDamage;

	CLog::Print(L"Poision Overlap!!");
	CLog::Print(SecondDamage);
}

void UCUpset_Poision::SetOriginMaterial(UMaterialInterface * Mat)
{
	//#Edit 0509 -
	//@우선은 Material 값을 check 함으로써 Material 은 무조건 유효한 값이라 정의
	check(Mat);
	if (Mat != nullptr)
	{
		OriginMaterial = Mat;
	}
}
