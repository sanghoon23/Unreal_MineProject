#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseActionComp.h"
#include "CPL_SwordActionComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordActionComp 
	: public UC_BaseActionComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCPL_SwordActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	IIC_BaseAction* GetIBaseAction(uint8 Type) override;
};
