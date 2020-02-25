#pragma once

#include "CoreMinimal.h"
#include "Actor/Base/CBaseInteractActor.h"
#include "CInteractActor_JumpOver.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACInteractActor_JumpOver 
	: public ACBaseInteractActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:


	#pragma endregion

public:
	ACInteractActor_JumpOver();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Virtual Function */
public:
	virtual void BeginInteract(AActor* InteractedActor) override;
	virtual void TickInteract(AActor* InteractedActor) override;
	virtual void EndInteract(AActor* InteractedActor) override;

	#pragma	region Member
private:

	#pragma endregion
};
