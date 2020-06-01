#include "CN_BeginBeated.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCN_BeginBeated::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
	IfNullRet(I_HitComp);

	I_HitComp->SetBeated(true);
}
