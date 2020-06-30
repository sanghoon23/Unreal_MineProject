#include "CDamageType_AirAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Component/Base/C_BaseHitComp.h"
#include "Interface/IC_Charactor.h"

UCDamageType_AirAttack::UCDamageType_AirAttack()
{
	// Super
	{
		TypeNumber = 2;
		DamageType = FDamageType::AIRATTACK;
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

void UCDamageType_AirAttack::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@때린 대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
	if (Charactor != nullptr)
	{
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
		check(I_Charactor);

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		I_Charactor->OffGravity();
	}

	//@Take Damage
	if (DamagedActorHitComp->IsDamagedFromOther() == true)
	{
		APawn* DamagedPawn = Cast<APawn>(DamagedActor);
		if (DamagedPawn != nullptr)
		{
			AController* PawnController = Cast<AController>(Cast<APawn>(DamagedActor));

			FDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = GetClass();
			DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);
		}
	}

	//@Motage
	{
		IfTrueRet(DamagedActorHitComp->IsBlockDamagedMontage());

		//@콤보가 가능한지,
		if (DamagedActorHitComp->IsCanHittedCombo() == true)
		{
			DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::COMBO, 0, 0.6f, true);
		}
		else //@해당 데미지에 해당하는 몽타주 실행
		{
			const uint8 MontageNum = static_cast<uint8>(GetConditionType());
			DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::NONE, MontageNum, 0.6f, true);
		}
	}
}

void UCDamageType_AirAttack::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}


