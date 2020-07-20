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

	// Attack
	IIC_BaseAttack* BaseAttack = I_AttackComp->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		BaseAttack->AttackOtherPawn();
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
				BaseAttack->AttackOtherPawn();
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

	// Attack
	// Charactor->GetAttackStateComp()->GetCurrentAttackState()->CheckAttack(MeshComp->GetOwner());

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bImpulse = false;
}

