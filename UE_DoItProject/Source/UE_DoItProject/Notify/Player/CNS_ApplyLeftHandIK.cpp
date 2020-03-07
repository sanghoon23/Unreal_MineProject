#include "CNS_ApplyLeftHandIK.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"

void UCNS_ApplyLeftHandIK::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	// @ON IK - LeftHand == 0
	Player->OnHandIK(0);
}

void UCNS_ApplyLeftHandIK::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_ApplyLeftHandIK::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	// @OFF IK - LeftHand == 0
	Player->OffHandIK(0);
}

