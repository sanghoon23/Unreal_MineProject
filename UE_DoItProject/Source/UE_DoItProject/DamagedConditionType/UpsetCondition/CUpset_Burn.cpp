#include "CUpset_Burn.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_MeshParticle.h"

UCUpset_Burn::UCUpset_Burn()
{
	//Super
	State = EHitUpset::BURN;
}

void UCUpset_Burn::StartCondition(APawn * Owner)
{
	Super::StartCondition(Owner);
	check(Owner);
	
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			check(BurnParticle);
			BurnParticleComp = I_MeshParticle->SpawnParticleAtMesh
			(
				BurnParticle,
				EAttachPointType::BODY,
				EAttachPointRelative::RELATIVE,
				EAttachLocation::SnapToTarget
			);

			if (BurnParticleComp == nullptr)
				UE_LOG(LogTemp, Warning, L"CUpset_Burn BurnParticleComp NULL!!");
		}
	}//(I_Charactor != nullptr)
}

void UCUpset_Burn::UpdateCondition(APawn * Owner, float DeltaTime)
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

	////@Particle Burn Update
	//IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	//if (I_Charactor != nullptr)
	//{
	//	//@만약 도중에 죽었다면,
	//	if (I_Charactor->IsDeath() == true)
	//	{
	//		ApplyTime = -0.001f;
	//	}
	//}
}

void UCUpset_Burn::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	//@Particle OFF
	if (BurnParticleComp != nullptr)
	{
		BurnParticleComp->SetActive(false);
		BurnParticleComp = nullptr;
	}
	else
		UE_LOG(LogTemp, Warning, L"CUpset_Burn EndCondition BurnParticleComp NULL!!");
}

void UCUpset_Burn::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Burn* OverlapCondition = Cast< UCUpset_Burn>(OverlappedCondition);
	check(OverlapCondition);

	//@초기화
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();
	AController* InputController = OverlapCondition->GetDamageSubjectController();
	if (GetDamageSubjectController() != InputController)
	{
		SetDamageSubjectController(InputController);
	}

	//@Damage 중첩
	const float InputConditionDamage = OverlapCondition->GetSecondDamage();
	SecondDamage += InputConditionDamage;
}

void UCUpset_Burn::SetBurnParticle(UParticleSystem * PT)
{
	//#Edit 0509 - 
	//@우선은 check 로써, BurnParticleComp 는 무조건 유효한 값으로 정의함
	check(PT);
	if (PT != nullptr)
	{
		BurnParticle = PT;
	}
}
