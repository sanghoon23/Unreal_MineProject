#include "CNS_DoingAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_DoingAttack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// Attack
	Charactor->GetIAttackComp()->GetCurrentIBaseAttack()->AttackOtherPawn();
}

void UCNS_DoingAttack::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// Attack Impulse
	if (bImpulse == false)
	{
		Charactor->GetIAttackComp()->GetCurrentIBaseAttack()->ImpulseAttack(0.5f);
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

