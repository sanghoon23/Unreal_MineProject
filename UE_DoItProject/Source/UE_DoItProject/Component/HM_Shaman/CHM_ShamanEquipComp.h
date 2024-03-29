#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_EquipComp.h"
#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Hand.h"

#include "CHM_ShamanEquipComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCHM_ShamanEquipComp 
	: public UActorComponent, public IIC_EquipComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		TArray<ACItem_Hand*> DisplayList;

	UPROPERTY(VisibleAnywhere, Category = "Item")
		class ACHM_ShamanMagicStick* MagicStick;

	#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
		void OnMeshVisible();

	UFUNCTION(BlueprintCallable)
		void OffMeshVisible();

public:	
	UCHM_ShamanEquipComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
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
	class ACItem_Hand*		CurrentWeapon;

	TMap<UItemType, TArray<class ACItem_Hand*> > DisplayMap;
	FVector AttachTransform;

	#pragma endregion

};
