#include "CNS_DoingAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_DoingAttack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRet(I_AttackComp);

	FString Path = L"";

	// Attack
	IIC_BaseAttack* BaseAttack = I_AttackComp->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		UCDamageType_Base* InputDamageType = NewObject<UCDamageType_Base>(this, DamageTypeClass);
		check(InputDamageType);
		
		//@Hit Move
		InputDamageType->SetHitMoveSpeed(HitMoveSpeed);

		//@Setting Data
		FDamageData Data;
		Data.DamageImpulse = DamageImpulse;
		Data.AirHeight = DamageAirHeight;
		Data.StunTime = DamageStunTime;
		Data.BurnTime = DamageBurnTime;
		Data.BurnSecondDamageValue = BurnSecondDamageValue;
		Data.PoisionTime = DamagePoisionTime;
		Data.PoisionSecondDamageValue = PoisionSecondDamageValue;
		Data.FreezingTime = DamageFreezingTime;

		InputDamageType->SettingData(Data);

		BaseAttack->AttackOtherPawn(InputDamageType);

		////@1.
		//UCDamageType_Base* InputDamageType = NewObject<UCDamageType_Base>(this, DamageTypeClass);
		//check(InputDamageType);
		//BaseAttack->AttackOtherPawn(InputDamageType);

		//@2.
		//FName strPath = L"";
		//UCDamageType_Base* InputDamageType = <UCDamageType_Base>(strPath);
		//UCDamageType_Base* InputDamageType =
		//	Cast<UCDamageType_Base>(StaticLoadObject(DamageTypeClass, NULL, *DamageType_strPath.ToString()));
		//check(InputDamageType);
		//BaseAttack->AttackOtherPawn(InputDamageType);
	}

	//@Init
	AttackTimer = 0.0f;
}

void UCNS_DoingAttack::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRet(I_AttackComp);

	if (bOnRunningTickOtherAttack == true)
	{
		AttackTimer += FrameDeltaTime;
		if (OtherAttackTimerRange < AttackTimer)
		{
			IIC_BaseAttack* BaseAttack = I_AttackComp->GetCurrentIBaseAttack();
			if (BaseAttack != nullptr)
			{
				UCDamageType_Base* InputDamageType = NewObject<UCDamageType_Base>(this, DamageTypeClass);
				check(InputDamageType);

				FDamageData Data;
				Data.DamageImpulse = DamageImpulse;
				Data.AirHeight = DamageAirHeight;
				Data.StunTime = DamageStunTime;
				Data.BurnTime = DamageBurnTime;
				Data.BurnSecondDamageValue = BurnSecondDamageValue;
				Data.PoisionTime = DamagePoisionTime;
				Data.PoisionSecondDamageValue = PoisionSecondDamageValue;
				Data.FreezingTime = DamageFreezingTime;

				InputDamageType->SettingData(Data);

				BaseAttack->AttackOtherPawn(InputDamageType);
			}
			AttackTimer = 0.0f;
		}
	}

	// Attack Impulse
	if (bImpulse == false)
	{
		I_AttackComp->GetCurrentIBaseAttack()->ImpulseAttack(0.5f);
		bImpulse = true;
	}
}

void UCNS_DoingAttack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bImpulse = false;
}

