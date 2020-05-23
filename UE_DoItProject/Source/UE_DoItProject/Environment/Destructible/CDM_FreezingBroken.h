#pragma once

#include "CoreMinimal.h"
#include "Environment/Destructible/Base/CDM_MeshActor.h"
#include "CDM_FreezingBroken.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACDM_FreezingBroken 
	: public ACDM_MeshActor
{
	GENERATED_BODY()

	#pragma	region Reflection

private:
	UFUNCTION()
		void OnFracture(const FVector& HitPoint, const FVector& HitDirection);

	#pragma endregion

public:
	ACDM_FreezingBroken();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
