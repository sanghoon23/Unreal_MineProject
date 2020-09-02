#include "CN_LookAtAttacker.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCN_LookAtAttacker::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IfNullRet(MeshComp);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
	IfNullRet(I_HitComp);

	const AActor* Attacker = I_HitComp->GetAttacker();
	IfNullRet(Attacker);

	//CLog::Print(MeshComp->GetOwner()->GetName());

	UCFL_ActorAgainst::LookAtTarget(MeshComp->GetOwner(), Attacker);
}