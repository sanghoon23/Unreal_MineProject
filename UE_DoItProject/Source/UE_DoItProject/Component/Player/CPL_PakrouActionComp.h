#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseActionComp.h"
#include "State/Player/Base/CPL_PakrouBaseAction.h"

#include "CPL_PakrouActionComp.generated.h"

UENUM()
enum class PakrouStartActionType : uint8
{
	JUMPOVER = 0,
	IDLE = 1,
	WALLCLIMBRUN = 2,
};

UENUM()
enum class PakrouLinkActionType : uint8
{
	IDLE = 0,
	LEFT = 1,
	RIGHT = 2,
	CLIMBOVER = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_PakrouActionComp 
	: public UC_BaseActionComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "PakrouBaseAction")
		TArray<UCPL_PakrouBaseAction*> StartActionArray;

	UPROPERTY(VisibleAnywhere, Category = "PakrouBaseAction")
		TArray<UCPL_PakrouBaseAction*> PakrouLinkAction;

	#pragma endregion

public:
	UCPL_PakrouActionComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Function */
public:
	void StartAction(PakrouStartActionType Type, AActor* Target);
	void DoLinkAction(PakrouLinkActionType Type, AActor* Target);
};
