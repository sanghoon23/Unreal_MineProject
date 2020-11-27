#include "CDM_LockWall.h"
#include "Global.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

ACDM_LockWall::ACDM_LockWall()
{
	PrimaryActorTick.bCanEverTick = true;

	FString hitOutObject = "Floor";
	HitOutString.Add(hitOutObject);

	// Super Setting
	{
		HitAfterLifeTime = 2.5f;

		GetDestructibleComponent()->SetGenerateOverlapEvents(true);
		GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//#1127_DestructibleMesh �Ӽ�â�� RandomSeed ���� �������� �ذ�.
		//������ �ذ��� ����� ���� CollisionSetting �� Overlap �Ѱ��ϻ���.
		//GetDestructibleComponent()->LargeChunkThreshold = 2000.0f;

		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetGenerateOverlapEvents(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	FString strPath = L"";

	// Set Destructible Mesh
	{
		strPath = L"DestructibleMesh'/Game/_Mine/Mesh/Environment/Destructible/DM_LockWall.DM_LockWall'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> DMMesh(*strPath);
		if (DMMesh.Succeeded())
		{
			GetDestructibleComponent()->SetSkeletalMesh(DMMesh.Object);
		}
	}
}

void ACDM_LockWall::BeginPlay()
{
	Super::BeginPlay();

	GetDestructibleComponent()->OnComponentFracture.AddDynamic(this, &ACDM_LockWall::OnFracture);
}

void ACDM_LockWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDM_LockWall::OnFracture(const FVector & HitPoint, const FVector & HitDirection)
{
	GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_LockWall::Death, HitAfterLifeTime);

}
