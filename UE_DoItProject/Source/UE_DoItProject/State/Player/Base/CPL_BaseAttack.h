#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPL_BaseAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPL_BaseAttack 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPL_BaseAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

	#pragma	region Member
private:

	#pragma endregion
};
