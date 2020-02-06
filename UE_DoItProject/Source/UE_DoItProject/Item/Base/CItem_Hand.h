#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CDisplayItem.h"
#include "CItem_Hand.generated.h"

UENUM()
enum class UHandItemType : uint8
{
	SWORD = 0,
	END = 1,
};

UCLASS()
class UE_DOITPROJECT_API ACItem_Hand 
	: public ACDisplayItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		UHandItemType HandItemType = UHandItemType::END;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AttachMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* DetachMontage;

public:
	ACItem_Hand();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void MeshAttach(FString StrAttachName) override; // ���� ���̱�.
	virtual void MeshDetach() override; // ���� ����..

	virtual void OnEquip();
	virtual void UnEquip();

};
