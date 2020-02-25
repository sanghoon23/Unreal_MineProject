#include "CNS_Interactive.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_InteractActor.h"

void UCNS_Interactive::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	// 이동 불가.
	I_Charactor->CanNotMove();

	IIC_InteractActor* I_InteractActor = I_Charactor->GetCurrentInteractActor();
	if (I_InteractActor != nullptr)
	{
		I_InteractActor->BeginInteract(Charactor);
	}
}

void UCNS_Interactive::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	IIC_InteractActor* I_InteractActor = I_Charactor->GetCurrentInteractActor();
	if (I_InteractActor != nullptr)
	{
		I_InteractActor->TickInteract(Charactor);
	}
}

void UCNS_Interactive::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	// 이동 불가 해제.
	I_Charactor->CanMove();

	IIC_InteractActor* I_InteractActor = I_Charactor->GetCurrentInteractActor();
	if (I_InteractActor != nullptr)
	{
		I_InteractActor->EndInteract(Charactor);
	}
}


