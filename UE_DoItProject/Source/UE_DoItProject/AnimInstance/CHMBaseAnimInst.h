#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
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
		bool bIsRunningMontage;

	UPROPERTY(BlueprintReadOnly)
		bool bDeath;

	UPROPERTY(BlueprintReadOnly)
		bool bAttackMode;

	#pragma endregion

public:
	UCHMBaseAnimInst();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Member
private:
	ACHumanoidMonster* Monster;
	IIC_Charactor* I_Charactor;
	IIC_Monster* I_Monster;
};
