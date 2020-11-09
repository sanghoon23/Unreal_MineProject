#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interface/IC_Player.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"

#include "CGameInst.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCGameInst 
	: public UGameInstance
{
	GENERATED_BODY()

#pragma region Reflection

private:
	UPROPERTY(VisibleAnywhere, Category = "GlobalData")
		TMap<int, FPlayerInfo> MapPlayerInfo;

	UPROPERTY(VisibleAnywhere, Category = "GlobalData")
		TArray<ACHumanoidMonster*> ExistWorldMonsterList;

#pragma endregion

public:
	virtual void Init() override;

public:
	void GetPlayerInfoFromId(FPlayerInfo& Info, int PlayerID);
	void SetPlayerInfoFromId(FPlayerInfo& Info, int PlayerID);

	const bool IsExistWorldMonster() { return (ExistWorldMonsterList.Num() != 0); }
	const int NumExistWorldMonster() { return ExistWorldMonsterList.Num(); }
	void AddExistWorldMonster(ACHumanoidMonster* InsertMon);


private:

};
