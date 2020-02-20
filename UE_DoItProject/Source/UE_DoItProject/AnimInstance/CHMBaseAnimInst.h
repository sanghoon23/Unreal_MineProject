#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Basic.h"

#include "CHMBaseAnimInst.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHMBaseAnimInst
	: public UAnimInstance
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

	UPROPERTY(BlueprintReadOnly)
		float Direction;

	UPROPERTY(BlueprintReadOnly)
		bool bDeath;

	#pragma endregion

public:
	UCHMBaseAnimInst();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Member
private:
	ACHM_Basic* Monster;
	IIC_Charactor* Charactor;
};
