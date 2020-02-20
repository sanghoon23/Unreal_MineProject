#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDisplayItem.generated.h"

UENUM()
enum class UItemType : uint8
{
	NONE = 0,
	HAND = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API ACDisplayItem : public AActor
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Attachment")
		FString ItemAttachName = "";

	UPROPERTY(EditAnywhere, Category = "Attachment")
		FString ItemEquipName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attachment")
		UItemType ItemType = UItemType::NONE;

	#pragma endregion

public:	
	ACDisplayItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	FString GetItemAttachName() { return ItemAttachName; }
	FString GetItemEquipName() { return ItemEquipName; }

	UItemType GetItemType() { return ItemType; }

	UCapsuleComponent* GetCapsuleComp() { return Capsule; }
	UStaticMeshComponent* GetStaticMeshComp() { return StaticMesh; }

	// Function
public:
	virtual void MeshAttach(FString StrAttachName);// 무기 붙이기.
	virtual void MeshDetach();// 원상 복구..

	#pragma	region Member
private:

	#pragma endregion
};
