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

	//@���� ��� �ٶ󺸱�
	//UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	APawn* DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* PawnController = Cast<APawn>(Subject)->GetController();
	check(PawnController);

	IIC_Charactor* const I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	check(I_Charactor);

	//@DamagedActor �� ���� ���� �� �ִٸ�,
	if (DamagedActorHitComp->IsDamagedFromOther() == true)
	{
		FDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = GetClass();
		DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, Subject);
	}

	//@ĳ���Ͱ� �׾��ٸ�,
	IfTrueRet(I_Charactor->IsDeath());

	//@Motage
	{
		//@DamageTypeEffet �� ������� �ʴ´ٸ�, Damage ��, ����.
		const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
		IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

		//@Montage ���� �� �� ���� �������� Ȯ��.
		IfTrueRet(I_Charactor->IsDontMontagePlay());

		const uint8 MontageNum = static_cast<uint8>(GetConditionType());
		DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::NONE, MontageNum, 0.6f, true);
	}
}

void UCDamageType_StrongAttack::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
}
