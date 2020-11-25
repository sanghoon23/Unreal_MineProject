#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Hand.h"
#include "CHM_MaoBlade.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_MaoBlade 
	: public ACItem_Hand
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
		class USceneComponent* Scene;

	#pragma endregion

public:
	ACHM_MaoBlade();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetMeshVisible(bool bSet) override;

public:
	FString GetItemAttachName() { return ItemAttachName; }
	
};
