#include "CNS_ApplyRightHandIK.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"

void UCNS_ApplyRightHandIK::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	// @ON IK - RightHand == 1
	Player->OnHandIK(1);
}

void UCNS_ApplyRightHandIK::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_ApplyRightHandIK::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	// @OFF IK - RightHand == 1
	Player->OffHandIK(1);
}

