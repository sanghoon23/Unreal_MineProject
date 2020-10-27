#include "CNS_EvadeMove.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_EvadeMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	I_Charactor->CanNotMove();
	Direction = I_Charactor->GetEvadeDirection();

	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	if (Charactor != nullptr)
	{
		CharMovementMaxSpeedValue = Charactor->GetCharacterMovement()->MaxWalkSpeed;
		CharMovementMaxSpeedValue *= 0.001f;
	}
}

void UCNS_EvadeMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Actor = MeshComp->GetOwner();
	IfNullRet(Actor);

	/*
	CharacterMovement 기준값,
	ex) AbilitySpeedUpper 로 최대속력이 늘어났을 때, 대쉬기의 속도도 같이 늘어나기 위해 상관관계 형성.
	*/
	FVector Vec = Actor->GetActorLocation();
	Vec = Vec + (Direction * Speed * CharMovementMaxSpeedValue);
	Actor->SetActorLocation(Vec);
}

void UCNS_EvadeMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	I_Charactor->CanMove();
	I_Charactor->OffEvade();
	I_Charactor->OnCollision();
}

