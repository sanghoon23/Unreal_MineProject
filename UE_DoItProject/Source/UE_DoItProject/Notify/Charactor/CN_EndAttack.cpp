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

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRet(I_AttackComp);

	IIC_BaseAttack* BaseAttackState = I_AttackComp->GetCurrentIBaseAttack();
	IfNullRet(BaseAttackState);

	if (BaseAttackState->EndAttackDeleFunc.IsBound() == true)
	{
		BaseAttackState->EndAttackDeleFunc.Broadcast();
	}
	else
	{
		CLog::Print(L"CN_EndAttack Notify EndAttackDeleFunc Not Bound!!");
	}
}
