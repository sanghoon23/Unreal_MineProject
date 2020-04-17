#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDecalActor_WithMouse.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACDecalActor_WithMouse 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UDecalComponent* DecalComp;

	#pragma endregion

	
public:	
	ACDecalActor_WithMouse();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void SetDecalCompMat(class UMaterialInterface* Material);
	void SetDecalCompRotation(FRotator Rotate);
	void SetDecalSize(FVector Size);
};
