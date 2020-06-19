#include "CNS_BlockPossibleAI.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Monster.h"

void UCNS_BlockPossibleAI::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	//@AI BLOCK ON
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Charactor);
	IfNullRet(I_Monster);
	I_Monster->SetAIRunningPossible(true);
}

void UCNS_BlockPossibleAI::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_BlockPossibleAI::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	//@AI BLOCK OFF
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Charactor);
	IfNullRet(I_Monster);
	I_Monster->SetAIRunningPossible(false);
}

