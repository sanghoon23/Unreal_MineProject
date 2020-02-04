#include "CN_EndEvade.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

void UCN_EndEvade::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	Charactor->OffEvade();
}

