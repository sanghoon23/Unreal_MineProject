#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_EquipComp.h"
#include "Item/Base/CDisplayItem.h"

#include "CPL_EquipComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPL_EquipComp
	: public UActorComponent, public IIC_EquipComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		TArray<ACDisplayItem*> DisplayList;



	#pragma endregion

public:	
	UCPL_EquipComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void WeaponRelease() override;
	virtual void WeaponNextSwap(bool AscendingOrder) override;

	virtual bool GetEquiping() const override { return bEquiping; }
	virtual void SetEquiping(bool bValue) override { bEquiping = bValue; }

	virtual bool GetArmed() const override { return bArmed; }
	virtual int GetCurrentWeaponNum() const override { return CurrentWeaponNum; }

	virtual class ACDisplayItem* GetCurrentWeaponDisplay() override { return CurrentWeapon; }
	virtual class ACDisplayItem* GetDisplayItem(int WeaponArrayNum) override;

private:
	void ItemMapAdd(UItemType Type, class ACDisplayItem* CItem);
	class ACDisplayItem* ItemMapFind(UItemType Type, int FindItemNum);

	#pragma	region Member
public:

private:
	bool					bEquiping = false;
	bool					bArmed = false;

	int						CurrentWeaponNum = -1;
	class ACDisplayItem*	CurrentWeapon;

	TMap<UItemType, TArray<class ACDisplayItem*>> DisplayMap;

	class ACPlayer* Player;
	FVector AttachTransform;

	#pragma endregion
};
