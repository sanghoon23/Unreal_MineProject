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

	// Mesh Min Max ���ϱ�
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
	//	// @�߷� ����
	//	Charactor->OffGravity();
	//}

	//// 1.1 Mesh Min Max ���ϱ� - GetLocalBounds
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

	//// 1. Mesh ���� �Ÿ� ���߱� ->SetTargetDistanceToThis �ȿ� SetActorLocation ȣ���ؼ� �������־�� ��.
	//APawn* InteractedPawn = Cast<APawn>(InteractedActor);
	//check(InteractedPawn);
	//SetTargetDistanceToThis(InteractedPawn);

	//// 2. ���� ���߱�
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

