#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Hand.h"
#include "CPL_Sword.generated.h"


UCLASS()
class UE_DOITPROJECT_API ACPL_Sword 
	: public ACItem_Hand
{
	GENERATED_BODY()

public:
	ACPL_Sword();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetMeshVisible(bool bSet) override;

public:
	FString GetItemAttachName() { return ItemAttachName; }
};
