#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerMontageComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPlayerMontageComp : public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPlayerMontageComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

	#pragma	region Member
private:

	#pragma endregion
};
