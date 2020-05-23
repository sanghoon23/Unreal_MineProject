#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_EquipComp.h"
#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Hand.h"

#include "CPL_EquipComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPL_EquipComp
	: public UActorComponent, public IIC_EquipComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		TArray<ACItem_Hand*> DisplayList;

	#pragma endregion

public:	
	UCPL_EquipComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void WeaponRelease() override;
	void WeaponNextSwap(bool AscendingOrder) override;

	bool GetEquiping() const override { return bEquiping; }
	void SetEquiping(bool bValue) override { bEquiping = bValue; }

	bool GetArmed() const override { return bArmed; }
	int GetCurrentWeaponNum() const override { return CurrentWeaponNum; }

	class ACItem_Hand* GetCurrentWeaponDisplay() override { return CurrentWeapon; }
	class ACItem_Hand* GetDisplayItem(int WeaponArrayNum) override;

private:
	void ItemMapAdd(UItemType Type, class ACItem_Hand* CItem);
	class ACItem_Hand* ItemMapFind(UItemType Type, int FindItemNum);

	#pragma	region Member
public:

private:
	bool					bEquiping = false;
	bool					bArmed = false;

	int						CurrentWeaponNum = -1;
	class ACItem_Hand*		CurrentWeapon;

	TMap<UItemType, TArray<class ACItem_Hand*>> DisplayMap;

	class ACPlayer* Player;
	FVector AttachTransform;

	#pragma endregion
};
