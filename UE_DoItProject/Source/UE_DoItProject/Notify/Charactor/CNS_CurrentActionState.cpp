#include "CNS_CurrentActionState.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAction.h"

void UCNS_CurrentActionState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// @Begin
	Charactor->GetCurrentIBaseAction()->BeginActionState();
}

void UCNS_CurrentActionState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// @Tick
	Charactor->GetCurrentIBaseAction()->TickActionState();
}

void UCNS_CurrentActionState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// @End
	Charactor->GetCurrentIBaseAction()->EndActionState();
}
