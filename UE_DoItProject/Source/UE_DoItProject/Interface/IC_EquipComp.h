#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Hand.h"

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

	/*@���� �����ϰ� �ִ� DisplayItem �� ������. */
	virtual class ACItem_Hand* GetCurrentWeaponDisplay() = 0;

	// 
	// @WeaponArrayNum - ������ Item �� Array Number
	// @Return - Item �� �ֻ��� �θ�
	// @Warning - DisplayList Size üũ���־����.
	virtual class ACItem_Hand* GetDisplayItem(int WeaponArrayNum) = 0;

	virtual bool GetEquiping() const = 0;
	virtual void SetEquiping(bool bValue) = 0;

	virtual bool GetArmed() const = 0;
	virtual int GetCurrentWeaponNum() const = 0;

};
