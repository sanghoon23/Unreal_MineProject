#include "CNS_EvadeMove.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_EvadeMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	Charactor->CanNotMove();
	Speed = Charactor->GetEvadeSpeed();
	Direction = Charactor->GetEvadeDirection();
}

void UCNS_EvadeMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Actor = MeshComp->GetOwner();
	IfNullRet(Actor);
	
	// @Evade Tick
	FVector Vec = Actor->GetActorLocation();
	Vec = Vec + (Direction * Speed);
	Actor->SetActorLocation(Vec);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);
}

void UCNS_EvadeMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	// @EvadeEnd
	Charactor->CanMove();
	Charactor->OffEvade();
}

