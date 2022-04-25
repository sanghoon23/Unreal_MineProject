#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"

#include "CMonsterSpawner.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACMonsterSpawner 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
	//UPROPERTY(BlueprintAssignable, Category = "Delegate")
	//	FDeadSpawningMS DelDeadSpawningMS;

private:
	UPROPERTY(EditAnywhere, Category = "Sequencer")
		class ULevelSequence* SeqMonsterSpawn;

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		class ULevelSequencePlayer* MonsterSpawnSequencePlayer;

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		class ULevelSequence* SeqNextStage;

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		class ULevelSequencePlayer* NextStageSequencePlayer;

	UPROPERTY(EditAnywhere, Category = "MonsterList")
		TArray<ACHumanoidMonster*> SeqMonsterSpawnList;

	UPROPERTY(EditAnywhere, Category = "Data")
		float FuncTimerOfNotExistMon = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Data")
		float DetectRadius = 1000.0f;

private:
	UFUNCTION()
		void TimerFunc();

	UFUNCTION()
		void SpawnSequencePlay();

	UFUNCTION()
		void SpawnSequenceFinished();

	UFUNCTION()
		void DeadSequencePlay();

	UFUNCTION()
		void DeadSequenceFinished();

	#pragma endregion
	
public:	
	ACMonsterSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	/* Player 와 맞닿았는지 */
	const bool IsTouchOurPlayer() const { return bHitting; }

	/* Player 가 이 단계를 끝냈는지 */
	const bool IsExitingOurPlayer() const { return bExit; }

	void CheckHittingInDetectRadius();

private:
	bool bExit = false;
	bool bHitting = false;
	AActor* HittingPlayer = nullptr;
};
