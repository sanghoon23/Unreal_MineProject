#include "CDM_MeshActor.h"
#include "Global.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

ACDM_MeshActor::ACDM_MeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//FString hitOutObject = "Floor";
	//HitOutString.Add(hitOutObject);

	//@Create
	{
		BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		BoxComp->SetupAttachment(GetDestructibleComponent());
	}

	// Setting
	{
		GetDestructibleComponent()->SetGenerateOverlapEvents(true);
		// 청크를 라지로 처리하는 데 필요한 최소 크기.
		// 일정 값이 넘어가면 플레이어가 통과 가능함; - 안됨.
		// LargeChunkThreshold 로 Collision 을 조정하지 말자.
		GetDestructibleComponent()->CanCharacterStepUpOn = ECB_No;
		GetDestructibleComponent()->LargeChunkThreshold = 70.0f;

		SetActorScale3D(FVector(1.0f));
	}

}

void ACDM_MeshActor::BeginPlay()
{
	Super::BeginPlay();

	//#Edit - 0516
	//@자식에서
	//GetDestructibleComponent()->OnComponentFracture.AddDynamic(this, &ACDM_MeshActor::OnFracture);
}

void ACDM_MeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ACDM_MeshActor::OnFracture(const FVector & HitPoint, const FVector & HitDirection)
//{
//	//@빠게졌을 때, Charactor 통과하게 하기 위해서
//	// #Edit - 0516
//	// LargeChunkThreshold 를 조정하면 된다고하는데, 아무리해도 안됨;
//	//GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//
//	FTimerHandle TimerHandle;
//	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_MeshActor::Death, HitAfterLifeTime);
//}

void ACDM_MeshActor::Death()
{
	Destroy();
}