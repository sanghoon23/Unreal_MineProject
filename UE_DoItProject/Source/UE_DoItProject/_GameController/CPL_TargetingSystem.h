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
	const float FindDistance = 3000.0f; // 찾을 반경


	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Container")
		TArray<APawn*> SelectedMonsters;

	UPROPERTY(VisibleAnywhere, Category = "Decal")
		/* Ex) Target 이 설정되었을 때 표시될 Decal */
		class ACDecalActor_Targeting* TargetDecal;



	#pragma endregion

public:	
	UCPL_TargetingSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	void OnFindTargets();
		
private:
	//void SelectedMonster

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

	/* DecalActor 의 원 반경 */
	FVector DecalActorCircleSize = FVector(300.0f);

	#pragma endregion
};
