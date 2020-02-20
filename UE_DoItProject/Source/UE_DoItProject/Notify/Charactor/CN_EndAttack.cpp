#include "CN_EndAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCN_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_BaseAttack* BaseAttackState = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	IfNullRet(BaseAttackState);

	BaseAttackState->EndAttackDeleFunc.Broadcast();
}
