#include "CN_BeginAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

void UCN_BeginAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRet(I_AttackComp);

	IIC_BaseAttack* BaseAttackState = I_AttackComp->GetCurrentIBaseAttack();
	IfNullRet(BaseAttackState);

	if (BaseAttackState->BeginAttackDeleFunc.IsBound() == true)
	{
		BaseAttackState->BeginAttackDeleFunc.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"CN_BeginAttack Notify BeginAttackDeleFunc Not Bound!!");
	}
}
