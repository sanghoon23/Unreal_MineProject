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
private:
	UPROPERTY(VisibleAnywhere, Category = "Sequencer")
		class ULevelSequence* SeqMonsterSpawn;

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		class ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(EditAnywhere, Category = "MonsterList")
		TArray<ACHumanoidMonster*> SeqMonsterSpawnList;

	UPROPERTY(EditAnywhere, Category = "Data")
		float DetectRadius = 1000.0f;

private:
	UFUNCTION()
		void SequenceFinished();

	#pragma endregion

	
public:	
	ACMonsterSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	bool bHitting = false;
	AActor* HittingPlayer = nullptr;
};
