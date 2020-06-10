#include "CNS_CanHitCombo.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCNS_CanHitCombo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	IIC_HitComp* I_HitComp = Cast<IIC_HitComp>(I_Charactor->GetIHitComp());
	IfNullRet(I_HitComp);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	I_HitComp->SetCanHittedCombo(true);
}

void UCNS_CanHitCombo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	IIC_HitComp* I_HitComp = Cast<IIC_HitComp>(I_Charactor->GetIHitComp());
	IfNullRet(I_HitComp);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	I_HitComp->SetCanHittedCombo(false);
}
