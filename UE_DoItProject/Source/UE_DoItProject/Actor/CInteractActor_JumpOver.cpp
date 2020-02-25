#include "CInteractActor_JumpOver.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"

ACInteractActor_JumpOver::ACInteractActor_JumpOver()
{
	PrimaryActorTick.bCanEverTick = true;

	// @IIC_InteractActor Value
	{
		Type = InteractType::JUMPOVER;
	}
}

void ACInteractActor_JumpOver::BeginPlay()
{
	Super::BeginPlay();
}

void ACInteractActor_JumpOver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ACInteractActor_JumpOver::BeginInteract(AActor * InteractedActor)
{
	Super::BeginInteract(InteractedActor);
	check(InteractedActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(InteractedActor);
	if (Charactor != nullptr)
	{
		// @중력 해제
		Charactor->OffGravity();
	}

	// 1.1 Mesh Min Max 구하기 - GetLocalBounds
	FVector Min;
	FVector Max;
	StaticMesh->GetLocalBounds(Min, Max);

	// 1.2 Mesh Scale
	float MeshScale = StaticMesh->GetComponentScale().Y;

	// 1.3 Half Height
	float HalfHeight = 0.0f;
	APawn* InteractedPawn = Cast<APawn>(InteractedActor);
	if (InteractedPawn != nullptr)
	{
		HalfHeight = InteractedPawn->GetDefaultHalfHeight();
	}

	// 2. 높이 맞추기
	FVector StaticMeshLocation = StaticMesh->GetComponentLocation();
	FVector ActorLocation = InteractedActor->GetActorLocation();
	ActorLocation.Z = (StaticMeshLocation.Z + (Max.Y * MeshScale) + HalfHeight);

	// 3. Setting
	InteractedActor->SetActorLocation(ActorLocation);
}

// @param Actor - 상호작용 '당할' Actor
// @Waring - bCollisioning Value 를 이용함
// Actor 가 Mesh 위에 올라서 있을 시간 동안에만 이동 시키기 위해서
void ACInteractActor_JumpOver::TickInteract(AActor * InteractedActor)
{
	Super::TickInteract(InteractedActor);

	// @계속 충돌 중이 아니면 리턴, Mesh 를 벗어낫기 때문에
	IfFalseRet(bCollisioning);

	CLog::Print(L"Tick In");

	// 1.1 Mesh Min Max 구하기 - GetLocalBounds
	FVector Min;
	FVector Max;
	StaticMesh->GetLocalBounds(Min, Max);

	// 1.2 Mesh Scale
	float MeshScale = StaticMesh->GetComponentScale().X;

	// 2. 앞으로 나아가기. - MeshScale * MeshScale (대략적인 수치 맞추기 위해)
	float Speed = InteractiveData.JumpOverSpeed;
	FVector ActorLocation = InteractedActor->GetActorLocation();
	FVector ActorForwardDir = InteractedActor->GetActorForwardVector();
	ActorLocation += (ActorForwardDir * Speed * (MeshScale * MeshScale));
	// ActorLocation += (ActorForwardDir * Speed * MeshScale);

	// 3. Setting
	InteractedActor->SetActorLocation(ActorLocation);

	// Test Code
	//CLog::Print(MeshScale);
	//CLog::Print(L"Min : " + Min.ToString());
	//CLog::Print(L"Max : " + Max.ToString());
}

void ACInteractActor_JumpOver::EndInteract(AActor * InteractedActor)
{
	Super::EndInteract(InteractedActor);
	check(InteractedActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(InteractedActor);
	if (Charactor != nullptr)
	{
		// @중력 원상 복구
		Charactor->OnGravity();
	}
}
