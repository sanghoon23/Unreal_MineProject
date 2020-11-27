#include "CDM_FreezingBroken.h"
#include "Global.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

ACDM_FreezingBroken::ACDM_FreezingBroken()
{
	PrimaryActorTick.bCanEverTick = true;

	FString hitOutObject = "Floor";
	HitOutString.Add(hitOutObject);

	// Super Setting
	{
		HitAfterLifeTime = 2.5f;

		GetDestructibleComponent()->SetGenerateOverlapEvents(false);
		GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// 청크를 라지로 처리하는 데 필요한 최소 크기.
		// - OnFracture CollisionEnabled 로 조정 - (의미없음)
		//#1127_DestructibleMesh 속성창에 RandomSeed 값을 높임으로 해결.
		//일전에 해결한 방식은 단지 CollisionSetting 을 Overlap 한것일뿐임.

		//GetDestructibleComponent()->LargeChunkThreshold = 2000.0f;

		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetGenerateOverlapEvents(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		SetActorScale3D(FVector(6.0f));
	}

	FString strPath = L"";

	// Set Destructible Mesh
	{
		strPath = L"DestructibleMesh'/Game/_Mine/Mesh/Environment/Destructible/Charactor/DM_FreezenActor.DM_FreezenActor'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> DMMesh(*strPath);
		if (DMMesh.Succeeded())
		{
			GetDestructibleComponent()->SetSkeletalMesh(DMMesh.Object);
		}
	}

	// Set Destructible MaterialInstance
	{
		strPath = L"MaterialInstanceConstant'/Game/_Mine/UseMaterial/Destructible/Mat_FreezingBroken_Inst.Mat_FreezingBroken_Inst'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> DM_Mat(*strPath);
		if (DM_Mat.Succeeded())
		{
			GetDestructibleComponent()->SetMaterial(0, DM_Mat.Object);
		}
	}
}

void ACDM_FreezingBroken::BeginPlay()
{
	Super::BeginPlay();

	GetDestructibleComponent()->OnComponentFracture.AddDynamic(this, &ACDM_FreezingBroken::OnFracture);
}

void ACDM_FreezingBroken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDM_FreezingBroken::OnFracture(const FVector & HitPoint, const FVector & HitDirection)
{
	//@빠게졌을 때, Charactor 통과하게 하기 위해서
	//#1127_
	//DestructibleMesh 속성창에 RandomSeed 값을 높임으로 해결.

	GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_FreezingBroken::Death, HitAfterLifeTime);
}
