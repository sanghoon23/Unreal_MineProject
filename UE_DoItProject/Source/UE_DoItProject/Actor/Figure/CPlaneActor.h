#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPlaneActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPlaneActor 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* MaterialInst;

	#pragma endregion

	
public:	
	ACPlaneActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void SetMaterial(class UMaterialInterface* Material);
	void SetSMVisibility(bool bValue);

};
