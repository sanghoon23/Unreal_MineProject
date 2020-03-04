#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_BaseActionComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UC_BaseActionComp 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:


	#pragma endregion

public:	
	UC_BaseActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


	#pragma	region Member
private:

	#pragma endregion
};
