#include "CInteractActor_Climb.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"

ACInteractActor_Climb::ACInteractActor_Climb()
{
	PrimaryActorTick.bCanEverTick = true;

	// @IIC_InteractActor Value
	{
		Type = InteractType::CLIMB;
	}	
}

void ACInteractActor_Climb::BeginPlay()
{
	Super::BeginPlay();

	// Mesh Min Max 구하기
	StaticMesh->GetLocalBounds(Min, Max);

	// Mesh Scale
	MeshScale = StaticMesh->GetComponentScale().Y;
}

void ACInteractActor_Climb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ACInteractActor_Climb::BeginInteract(AActor * InteractedActor)
{
	Super::BeginInteract(InteractedActor);
	check(InteractedActor);

	//IIC_Charactor* Charactor = Cast<IIC_Charactor>(InteractedActor);
	//if (Charactor != nullptr)
	//{
	//	// @중력 해제
	//	Charactor->OffGravity();
	//}

	//// 1.1 Mesh Min Max 구하기 - GetLocalBounds
	////FVector Min;
	////FVector Max;
	////StaticMesh->GetLocalBounds(Min, Max);
	//// 1.2 Mesh Scale
	//// float MeshScale = StaticMesh->GetComponentScale().Y;
	////// 1.1 Half Height
	////float HalfHeight = 0.0f;
	////APawn* InteractedPawn = Cast<APawn>(InteractedActor);
	////if (InteractedPawn != nullptr)
	////{
	////	HalfHeight = InteractedPawn->GetSimpleCollisionRadius() + 10.0f;
	////}

	//// 1. Mesh 와의 거리 맞추기 ->SetTargetDistanceToThis 안에 SetActorLocation 호출해서 먼저해주어야 함.
	//APawn* InteractedPawn = Cast<APawn>(InteractedActor);
	//check(InteractedPawn);
	//SetTargetDistanceToThis(InteractedPawn);

	//// 2. 높이 맞추기
	//FVector StaticMeshLocation = StaticMesh->GetComponentLocation();
	//FVector ActorLocation = InteractedActor->GetActorLocation();
	//ActorLocation.Z = (StaticMeshLocation.Z + (Max.Y * MeshScale) - 80.0f);

	//// 3. Setting
	//InteractedActor->SetActorLocation(ActorLocation);
}

void ACInteractActor_Climb::TickInteract(AActor * InteractedActor)
{
	Super::TickInteract(InteractedActor);
	check(InteractedActor);
}

void ACInteractActor_Climb::EndInteract(AActor * InteractedActor)
{
	Super::EndInteract(InteractedActor);
	check(InteractedActor);
}

