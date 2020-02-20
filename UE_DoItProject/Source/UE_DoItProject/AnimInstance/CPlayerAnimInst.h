#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

#include "CPlayerAnimInst.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPlayerAnimInst 
	: public UAnimInstance
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		float Direction;

	UPROPERTY(BlueprintReadOnly)
		int CurrentStateType;

	UPROPERTY(BlueprintReadOnly)
		bool bArmed;

	UPROPERTY(BlueprintReadOnly)
		bool bAttackMode;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

private:
	UFUNCTION()
		void Test(UAnimMontage* Montage, bool bInterrupted);

	#pragma endregion

public:
	UCPlayerAnimInst();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Member
private:
	ACPlayer* Player;
	IIC_Charactor* Charactor;

};
