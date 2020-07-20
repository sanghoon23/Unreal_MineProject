#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CNS_DoingAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCNS_DoingAttack : public UAnimNotifyState
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		/* Tick 에서도 공격 체크의 여부 */
		bool bOnRunningTickOtherAttack = false;

	UPROPERTY(EditAnywhere, Category = "Data")
		/* 공격 체크 시간 간격 */
		float OtherAttackTimerRange = 0.0f;

#pragma endregion

private:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	bool bImpulse = false;

	float AttackTimer = 0.0f;
};
