#include "CDamageType_Air.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Component/Base/C_BaseHitComp.h"
#include "Interface/IC_Charactor.h"

UCDamageType_Air::UCDamageType_Air()
{
	// Super
	{
		TypeNumber = 1;
		DamageType = FDamageType::AIR;
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

void UCDamageType_Air::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@���� ��� �ٶ󺸱�
	//UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	//ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
	//if (Charactor != nullptr)
	//{
	//	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	//	check(I_Charactor);

	//	//@Montage �� ������ �� �ִ� ��Ȳ�� �ƴ� ������ Ex) - ��� ��Ȳ�� �ƴ� ��,
	//	if (I_Charactor->IsDontMontagePlay() == false)
	//	{
	//		FVector Location = DamagedActor->GetActorLocation();
	//		Location.Z = HeightAfterAirAttack;
	//		DamagedActor->SetActorLocation(Location);

	//		// @�ӷ� ���̱� - �߷²��� �ٷ� �������
	//		Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

	//		// @�߷� ����.
	//		I_Charactor->OffGravity();
	//	}
	//}

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
		//@DamageTypeEffet �� ������� �ʴ´ٸ�, Damage ��, ����.
		const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
		IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

		ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
		if (Charactor != nullptr)
		{
			IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
			check(I_Charactor);
			IfTrueRet(I_Charactor->IsDontMontagePlay()); //@Ex) Freeze ��Ȳ�� ��, return

			//
			FVector Location = DamagedActor->GetActorLocation();
			Location.Z = HeightAfterAirAttack;
			DamagedActor->SetActorLocation(Location);

			// @�ӷ� ���̱� - �߷²��� �ٷ� �������
			Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

			// @�߷� ����.
			I_Charactor->OffGravity();
		}

		//@�޺��� ��������,
		if (DamagedActorHitComp->IsCanHittedCombo() == true)
		{
			DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::COMBO, 0, 0.6f, true);
		}
		else //@�ش� �������� �ش��ϴ� ��Ÿ�� ����
		{
			const uint8 MontageNum = static_cast<uint8>(GetConditionType());
			DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::NONE, MontageNum, 0.6f, true);
		}
	}
}

void UCDamageType_Air::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
	SetAirAttackHeight(Data.AirHeight);
}
