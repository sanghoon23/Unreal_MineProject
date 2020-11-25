#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Charactor/Player/CPlayer.h"

#include "MyPlayerState.generated.h"

UCLASS()
class UE_DOITPROJECT_API AMyPlayerState 
	: public APlayerState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
		FPlayerInfo PlayerInfo;

private:


	#pragma endregion
public:
	void GetPlayerInfo(FPlayerInfo& Out);
	void SetPlayerInfo(FPlayerInfo& Info);

	const float GetPlayerHealth() const { return PlayerInfo.CurrentHP; }
	
private:
};
