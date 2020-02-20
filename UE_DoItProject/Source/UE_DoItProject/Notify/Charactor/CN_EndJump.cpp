#include "CN_EndJump.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

void UCN_EndJump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	I_Charactor->OffJumping();

}

