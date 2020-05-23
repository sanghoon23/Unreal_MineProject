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
		// ûũ�� ������ ó���ϴ� �� �ʿ��� �ּ� ũ��.
		// ���� ���� �Ѿ�� �÷��̾ ��� ������; - �ȵ�.
		// LargeChunkThreshold �� Collision �� �������� ����.
		GetDestructibleComponent()->CanCharacterStepUpOn = ECB_No;
		GetDestructibleComponent()->LargeChunkThreshold = 70.0f;

		SetActorScale3D(FVector(1.0f));
	}

}

void ACDM_MeshActor::BeginPlay()
{
	Super::BeginPlay();

	//#Edit - 0516
	//@�ڽĿ���
	//GetDestructibleComponent()->OnComponentFracture.AddDynamic(this, &ACDM_MeshActor::OnFracture);
}

void ACDM_MeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ACDM_MeshActor::OnFracture(const FVector & HitPoint, const FVector & HitDirection)
//{
//	//@�������� ��, Charactor ����ϰ� �ϱ� ���ؼ�
//	// #Edit - 0516
//	// LargeChunkThreshold �� �����ϸ� �ȴٰ��ϴµ�, �ƹ����ص� �ȵ�;
//	//GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//
//	FTimerHandle TimerHandle;
//	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_MeshActor::Death, HitAfterLifeTime);
//}

void ACDM_MeshActor::Death()
{
	Destroy();
}