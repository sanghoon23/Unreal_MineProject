#include "CN_EndEquiping.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"

void UCN_EndEquiping::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);


	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	Charactor->GetIEquipComp()->SetEquiping(false);
}
