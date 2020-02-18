#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPL_TargetingSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPL_TargetingSystem 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPL_TargetingSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnFindTargets();
		

	#pragma	region Member
public:
	APawn* GetCurrentFindAttackTarget() { return FindAttackTarget; }

private:
	float FindDistance = 1500.0f; // 찾을 반경
	APawn* FindAttackTarget;

	#pragma endregion
};
