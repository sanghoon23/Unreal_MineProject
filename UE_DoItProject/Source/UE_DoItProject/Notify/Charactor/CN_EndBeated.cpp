#include "CN_EndBeated.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCN_EndBeated::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
	IfNullRet(I_HitComp);

	I_HitComp->SetBeated(false);

	I_HitComp->EndBeatedFunc.Broadcast(MeshComp->GetOwner());
	I_HitComp->EndBeatedFunc.Clear();
}
