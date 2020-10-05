#include "CDamageType_StrongAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"

UCDamageType_StrongAttack::UCDamageType_StrongAttack()
{
	// Super
	{
		TypeNumber = 3;
		DamageType = FDamageType::STRONGATTACK;
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
}

void UCDamageType_StrongAttack::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@때린 대상 바라보기
	//UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	//@Take Damage
	if (DamagedActorHitComp->IsDamagedFromOther() == true)
	{
		APawn* DamagedPawn = Cast<APawn>(DamagedActor);
		if (DamagedPawn != nullptr)
		{
			AController* PawnController = Cast<APawn>(Subject)->GetController();
			check(PawnController);

			FDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = GetClass();
			DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, Subject);
		}
	}

	//@Motage
	{
		//@DamageTypeEffet 를 사용하지 않는다면, Damage 만, 들어간다.
		const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
		IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

		ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
		if (Charactor != nullptr)
		{
			IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
			check(I_Charactor);
			IfTrueRet(I_Charactor->IsDontMontagePlay());
		}

		const uint8 MontageNum = static_cast<uint8>(GetConditionType());
		DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::NONE, MontageNum, 0.6f, true);
	}
}

void UCDamageType_StrongAttack::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
}
