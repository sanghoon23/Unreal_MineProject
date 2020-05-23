#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_EquipComp.h"
#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Hand.h"

#include "CHM_BasicEquipComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCHM_BasicEquipComp 
	: public UActorComponent, public IIC_EquipComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		TArray<ACItem_Hand*> DisplayList;

	UPROPERTY(VisibleAnywhere, Category = "Item")
		class ACHM_BasicSword* Sword;

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

	class ACItem_Hand* GetCurrentWeaponDisplay() override { return CurrentWeapon; }
	class ACItem_Hand* GetDisplayItem(int WeaponArrayNum) override;

	#pragma	region Member
private:
	bool					bEquiping = false;
	bool					bArmed = false;

	int						CurrentWeaponNum = -1;
	class ACItem_Hand*	CurrentWeapon;

	TMap<UItemType, TArray<class ACItem_Hand*> > DisplayMap;

	class ACPlayer* Player;
	FVector AttachTransform;

	#pragma endregion
};
