#include "CNS_ComboCheck.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_ComboCheck::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UCNS_ComboCheck::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_ComboCheck::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IIC_BaseAttack* BaseAttackState = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	IfNullRet(BaseAttackState);
	bool bDoNextCombo = BaseAttackState->GetComboCheck();

	if (bDoNextCombo == true)
	{
		BaseAttackState->OnComboSet(MeshComp->GetOwner());
	}
	else
	{
		BaseAttackState->EndAttackDeleFunc.Broadcast();
		// BaseAttackState->EndAttack(MeshComp->GetOwner());
	}

}
