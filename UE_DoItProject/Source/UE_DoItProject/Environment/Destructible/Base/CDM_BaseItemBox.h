#pragma once

#include "CoreMinimal.h"
#include "Environment/Destructible/Base/CDM_MeshActor.h"
#include "CDM_BaseItemBox.generated.h"



UCLASS()
class UE_DOITPROJECT_API ACDM_BaseItemBox 
	: public ACDM_MeshActor
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UFUNCTION()
		void OnFracture(const FVector& HitPoint, const FVector& HitDirection);

	#pragma endregion
	
public:
	ACDM_BaseItemBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Function */
protected:
	virtual void SpawnItem();

};
