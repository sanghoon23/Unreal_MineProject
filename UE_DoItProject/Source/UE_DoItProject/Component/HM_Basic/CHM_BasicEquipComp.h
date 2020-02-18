#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_EquipComp.h"
#include "Item/Base/CDisplayItem.h"

#include "CHM_BasicEquipComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCHM_BasicEquipComp 
	: public UActorComponent, public IIC_EquipComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		TArray<ACDisplayItem*> DisplayList;

	#pragma endregion

public:	
	UCHM_BasicEquipComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool GetEquiping() const override { return bEquiping; }
	void SetEquiping(bool bValue) override { bEquiping = bValue; }
		
	bool GetArmed() const override { return bArmed; }
	int GetCurrentWeaponNum() const override { return CurrentWeaponNum; }

	class ACDisplayItem* GetCurrentWeaponDisplay() override { return CurrentWeapon; }
	class ACDisplayItem* GetDisplayItem(int WeaponArrayNum) override;

	#pragma	region Member
private:
	bool					bEquiping = false;
	bool					bArmed = false;

	int						CurrentWeaponNum = -1;
	class ACDisplayItem*	CurrentWeapon;

	TMap<UItemType, TArray<class ACDisplayItem*> > DisplayMap;

	class ACPlayer* Player;
	FVector AttachTransform;

	#pragma endregion
};
