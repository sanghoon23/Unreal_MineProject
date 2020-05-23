#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Item/Display/CItemBox_MediumWood.h"

#include "CAnimInst_MediumWood.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCAnimInst_MediumWood 
	: public UAnimInstance
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	UPROPERTY(BlueprintReadOnly)
		bool bBoxOpen = false;

	#pragma endregion

public:
	UCAnimInst_MediumWood();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/* Member */
private:
	class ACItemBox_MediumWood* MediumWood;

};
