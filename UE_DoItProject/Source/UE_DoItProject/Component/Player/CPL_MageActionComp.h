#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseActionComp.h"
#include "CPL_MageActionComp.generated.h"

UENUM()
enum class MageActionType
{
	ROLL = 0,
	END = 1,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_MageActionComp 
	: public UC_BaseActionComp
{
	GENERATED_BODY()

	#pragma region Reflection
private:

	#pragma endregion

public:
	UCPL_MageActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	IIC_BaseAction* GetIBaseAction(uint8 Type) override;
};
