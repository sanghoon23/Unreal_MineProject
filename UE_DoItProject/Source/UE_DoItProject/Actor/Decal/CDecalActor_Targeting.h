#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDecalActor_Targeting.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACDecalActor_Targeting 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UDecalComponent* DecalComp;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* TargetingCircle;

	#pragma endregion

	
public:	
	ACDecalActor_Targeting();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void SetDecalCompMat(class UMaterialInterface* Material);
	void SetDecalCompRotation(FRotator Rotate);

	void SetDecalSize(float fSize);
	void SetDecalSize(FVector Size);
};
