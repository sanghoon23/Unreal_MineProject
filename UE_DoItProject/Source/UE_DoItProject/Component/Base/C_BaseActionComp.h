#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_ActionComp.h"
#include "State/Base/C_BaseActionState.h"

#include "C_BaseActionComp.generated.h"

UENUM()
enum class ActionType
{
	ROLL = 0,
	END = 1,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UC_BaseActionComp 
	: public UActorComponent, public IIC_ActionComp
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	TArray<UC_BaseActionState*> BaseActionArray;

	#pragma endregion

public:	
	UC_BaseActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Interface Virtual */
public:
	virtual IIC_BaseAction* GetIBaseAction(uint8 Type) override { return nullptr; }

	#pragma	region Member
private:

	#pragma endregion
};
