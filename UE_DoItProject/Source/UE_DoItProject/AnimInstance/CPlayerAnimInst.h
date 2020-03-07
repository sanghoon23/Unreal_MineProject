#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/CInverseKinematics.h"

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
		bool bCharactorJumping;

	// @착지에 필요한 땅과의 거리
	UPROPERTY(BlueprintReadOnly)
		float FindFloorDistance;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FFeetInverseKinematics FeetEffector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FHandsInverseKinematics HandEffector;

private:
	// Test Code
	//UFUNCTION()
	//	void Test(UAnimMontage* Montage, bool bInterrupted);

	#pragma endregion

public:
	UCPlayerAnimInst();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float FootTraceDistance();

	// Member
private:
	ACPlayer* Player;
	IIC_Charactor* Charactor;
	class UCInverseKinematics* FootIK;

};
