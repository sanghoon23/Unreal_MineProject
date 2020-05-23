#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDisplayItem.generated.h"

UENUM()
enum class UItemType : uint8
{
	NONE = 0,
	HAND = 1,
	EVENT = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API ACDisplayItem 
	: public AActor
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attachment")
		UItemType ItemType = UItemType::NONE;

	#pragma endregion

public:	
	ACDisplayItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Death();

	#pragma	region Member
public:
	UItemType GetItemType() { return ItemType; }

	UCapsuleComponent* GetCapsuleComp() { return Capsule; }
	UStaticMeshComponent* GetStaticMeshComp() { return StaticMesh; }

private:

	#pragma endregion
};
