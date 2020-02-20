#include "CN_BeginAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCN_BeginAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_BaseAttack* BaseAttackState = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	IfNullRet(BaseAttackState);

	CLog::Print(L"Begine Notify Call");
	BaseAttackState->BeginAttackDeleFunc.Broadcast();
}
