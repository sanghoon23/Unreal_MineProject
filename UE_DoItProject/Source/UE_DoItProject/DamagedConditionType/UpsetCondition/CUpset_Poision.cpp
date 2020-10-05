#include "CUpset_Poision.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

#include "Interface/IC_WidgetInfo.h"

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

		//@UI 표기 X
		IIC_WidgetInfo* I_Widget = Cast<IIC_WidgetInfo>(Owner);
		if (I_Widget != nullptr)
		{
			I_Widget->SetOnceNoneUsingFloatingCombo();
		}

		//@Take Damage
		Owner->TakeDamage(SecondDamage, DamageEvent, GetDamageSubjectController(), Causer);
	}
}

void UCUpset_Poision::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	check(I_Charactor);

	IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
	check(I_HitComp);

	I_HitComp->SettingCustomCharactorMesh(ECharactorMeshSort::ORIGIN);

	//Before
	//ACharacter* Charactor = Cast<ACharacter>(Owner);
	//if (Charactor != nullptr)
	//{
	//	//if (Map_OriginMaterial.Num() <= 0) return;

	//	//for (auto& Material : Map_OriginMaterial)
	//	//{
	//	//	Charactor->GetMesh()->SetMaterial(Material.Key, Material.Value);
	//	//}

	//	//Map_OriginMaterial.Empty();
	//}
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
}

void UCUpset_Poision::SetOriginMaterial(TMap<int32, class UMaterialInterface*>& In)
{
	for (auto& Material : In)
	{
		Map_OriginMaterial.Emplace(Material.Key, Material.Value);
	}
}

void UCUpset_Poision::SetOriginMaterial(int32 Index, UMaterialInterface * Mat)
{
	//#Edit 0619 - TMap 구현
	check(Mat);
	if (Mat != nullptr)
	{
		Map_OriginMaterial.Emplace(Index, Mat);
	}
}
