#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IC_InteractActor.h"

#include "CBaseInteractActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACBaseInteractActor 
	: public AActor, public IIC_InteractActor
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Attribute")
		FVector GripPoint;

	/* 다른 액터와 계속 충돌되고 있는지 확인하기 위해 */
	UFUNCTION()
		void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* 다른 액터와 계속 충돌되고 있는지 확인하기 위해 */
	UFUNCTION()
		void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	#pragma endregion

public:	
	ACBaseInteractActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Virtual Function */
public:
	virtual void BeginInteract(AActor* InteractedActor) override {}
	virtual void TickInteract(AActor* InteractedActor) override {}
	virtual void EndInteract(AActor* InteractedActor) override {}

	/* Function */
public:
	UStaticMeshComponent* GetStaticMeshComponent();

	#pragma	region Member
public:
	bool GetIsMeshCollisioning() const { return bCollisioning; }

protected:
	/* @InteractActor Collision 되고 있는지 - BeginOverlap */
	bool bCollisioning = false;

private:

	#pragma endregion
};
