#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Hand.h"
#include "CHM_ShamanMagicStick.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_ShamanMagicStick 
	: public ACItem_Hand
{
	GENERATED_BODY()
	
public:
	ACHM_ShamanMagicStick();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	FString GetItemAttachName() { return ItemAttachName; }

};
