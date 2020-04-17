#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_MGAttackMagicBall.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MGAttackMagicBall 
	: public UCPL_MageBaseAttack
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UFUNCTION(Category = "Timer")
		void TimerMontagePause();

	#pragma endregion

public:
	UCPL_MGAttackMagicBall();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
	/* Function */
public:
	void BeginAttack(AActor * DoingActor) override;
	//void EndAttack() override;

	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

private:
	class ACProjectile_MagicBall* SpawnMagicBall();

	#pragma region Member
private:
	float AttackRadius = 250.0f;

	class UWG_SkillCastingBar* SkillCastWidget;
	bool bFillingGauge = false;

	FTimerHandle MontagePauseTimer;
	FTimerDelegate MontagePauseDel;

	#pragma endregion
};
