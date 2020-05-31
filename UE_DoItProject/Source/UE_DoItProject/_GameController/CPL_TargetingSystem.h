#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPL_TargetingSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPL_TargetingSystem 
	: public UActorComponent
{
	GENERATED_BODY()

private:
	const float FindDistance = 3000.0f; // ã�� �ݰ�


	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Container")
		TArray<APawn*> SelectedMonsters;

	UPROPERTY(VisibleAnywhere, Category = "Actor")
		/* Ex) Target �� �����Ǿ��� �� ǥ�õ� Plane */
		class ACPlaneActor* TargetMarkActor;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* Mat_TargetMarkActor;

	#pragma endregion

public:	
	UCPL_TargetingSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	void OnFindTargets(FVector CenterPos, float CollisionSphereRadius);
		
private:
	bool CheckSelectedMonsters(APawn* InputPawn);
	void AddSelectedMonstersArray(APawn* InputPawn);
	void DelSelectedMonstersArray(APawn* InputPawn);
	APawn* GetSelectedMonsterArray(uint8 Index);

	#pragma	region Member
public:
	APawn* GetCurrentFindAttackTarget();

private:
	class ACPlayer* Player;

	class UWG_TargetInfo* TargetInfoWidget;

	uint8 CurrentIndexFindAttackTarget = 0;
	APawn* CurrentFindAttackTarget;

	#pragma endregion
};
