#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_Component.h"

#include "CPL_MGAttackFreezing.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MGAttackFreezing 
	: public UCPL_MageBaseAttack, public IIC_Component
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UFUNCTION(Category = "Timer")
		void TimerMontagePause();

	#pragma endregion
	
public:
	UCPL_MGAttackFreezing();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Pure Virtual Function - IC_BaseAttack */
public:
	void BeginAttack(AActor * DoingActor) override;
	//void EndAttack() override;

	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

	#pragma region Member
private:
	class UWG_SkillCastingBar* SkillCastWidget;
	bool bFillingGauge = false;
	float FillingSpeed = 1.0f;

	FTimerHandle MontagePauseTimer;
	float MontagePauseOffset = 2.0f;

	FTimerDelegate MontagePauseDel;

	TArray<class UCN_SpawnProjectile*> Notifies_SpawnProjectile;
	#pragma endregion
};
