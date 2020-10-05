#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AI/Controller/Base/CAIC_BaseHM.h"

#include "CAIC_HM_PengMao.generated.h"

UENUM(BlueprintType)
enum class EAIState_PengMao : uint8
{
	DISCOVERY = 0, //@�� �߰�
	WAITATTACK = 1, //@���� ���
	TRYATTACK = 2, //@���� �õ�
	FINDATTACKPOINT = 3, //@
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_PengMao 
	: public ACAIC_BaseHM
{
	GENERATED_BODY()

	#pragma region Reflection
//private:
//	UPROPERTY(EditAnywhere, Category = "AIControl")
//		float PatrolRadius = 400.0f;
//
//	UPROPERTY(EditAnywhere, Category = "AIControl")
//		float DetectRadius = 1500.0f;
//
//	UPROPERTY(EditAnywhere, Category = "AIControl")
//		float AttackRange = 500.0f;

	#pragma endregion

public:
	ACAIC_HM_PengMao();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

//private:
//	class UBehaviorTree* BT;
//	class UBlackboardData* BB;
	
};
