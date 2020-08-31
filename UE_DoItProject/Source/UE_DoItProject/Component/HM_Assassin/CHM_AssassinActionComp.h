#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseActionComp.h"
#include "Interface/IC_ActionComp.h"
#include "State/Base/C_BaseActionState.h"

#include "CHM_AssassinActionComp.generated.h"

UENUM()
enum class EAssa_ActionType
{
	ANGER = 0,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_AssassinActionComp 
	: public UC_BaseActionComp, public IIC_ActionComp
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "BaseAction")
		TArray<UC_BaseActionState*> ActionArray;

#pragma endregion

public:
	UCHM_AssassinActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	IIC_BaseAction* GetIBaseAction(uint8 Type) override;

};
