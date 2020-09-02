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

	//@때린 대상 바라보기
	//UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	//ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
	//if (Charactor != nullptr)
	//{
	//	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
	//	check(I_Charactor);

	//	//@Montage 를 실행할 수 있는 상황이 아닐 때에는 Ex) - 결빙 상황이 아닐 때,
	//	if (I_Charactor->IsDontMontagePlay() == false)
	//	{
	//		FVector Location = DamagedActor->GetActorLocation();
	//		Location.Z = HeightAfterAirAttack;
	//		DamagedActor->SetActorLocation(Location);

	//		// @속력 줄이기 - 중력끄고 바로 해줘야함
	//		Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

	//		// @중력 끄기.
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
		//@DamageTypeEffet 를 사용하지 않는다면, Damage 만, 들어간다.
		const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
		IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

		ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
		if (Charactor != nullptr)
		{
			IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
			check(I_Charactor);
			IfTrueRet(I_Charactor->IsDontMontagePlay()); //@Ex) Freeze 상황일 때, return

			//
			FVector Location = DamagedActor->GetActorLocation();
			Location.Z = HeightAfterAirAttack;
			DamagedActor->SetActorLocation(Location);

			// @속력 줄이기 - 중력끄고 바로 해줘야함
			Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

			// @중력 끄기.
			I_Charactor->OffGravity();
		}

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

void UCDamageType_Air::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
	SetAirAttackHeight(Data.AirHeight);
}
