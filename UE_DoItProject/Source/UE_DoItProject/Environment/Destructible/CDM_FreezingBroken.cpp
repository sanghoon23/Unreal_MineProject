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

		GetDestructibleComponent()->SetGenerateOverlapEvents(true);

		// ûũ�� ������ ó���ϴ� �� �ʿ��� �ּ� ũ��.
		// - OnFracture CollisionEnabled �� ����
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
	//@�������� ��, Charactor ����ϰ� �ϱ� ���ؼ�
	// #Edit - 0516
	// LargeChunkThreshold �� ���� - CollisionEnabled �� ����;

	GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_FreezingBroken::Death, HitAfterLifeTime);
}
