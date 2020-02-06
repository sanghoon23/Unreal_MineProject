#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item/Base/CDisplayItem.h"

#include "IC_EquipComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_EquipComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_EquipComp
{
	GENERATED_BODY()

public:
	virtual void WeaponRelease() {}
	virtual void WeaponNextSwap(bool AscendingOrder) {}

public:
	virtual class ACDisplayItem* GetCurrentWeaponDisplay() { return nullptr; }

	virtual bool GetEquiping() const { return false; }
	virtual void SetEquiping(bool bValue) {}

	virtual bool GetArmed() const { return false; }
	virtual int GetCurrentWeaponNum() const { return -1; }

};
