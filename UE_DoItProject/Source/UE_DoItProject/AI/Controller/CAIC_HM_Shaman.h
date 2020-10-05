#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AI/Controller/Base/CAIC_BaseHM.h"

#include "CAIC_HM_Shaman.generated.h"

UENUM(BlueprintType)
enum class EAIState_Shaman : uint8
{
	DISCOVERY = 0, //@적발견
	TRYATTACK = 1, //@공격 시도
	FINDATTACKPOINT = 2, //@
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_Shaman 
	: public ACAIC_BaseHM
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	//UPROPERTY(EditAnywhere, Category = "AIControl")
	//	float PatrolRadius = 400.0f;

	//UPROPERTY(EditAnywhere, Category = "AIControl")
	//	float DetectRadius = 900.0f;

	//UPROPERTY(EditAnywhere, Category = "AIControl")
	//	float AttackRange = 500.0f;

	#pragma endregion

public:
	ACAIC_HM_Shaman();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

//private:
//	class UBehaviorTree* BT;
//	class UBlackboardData* BB;


};
