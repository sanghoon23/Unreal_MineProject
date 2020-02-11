#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Hand.h"
#include "CHM_BasicSword.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_BasicSword : public ACItem_Hand
{
	GENERATED_BODY()
public:
	ACHM_BasicSword();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	FString GetItemAttachName() { return ItemAttachName; }
};
