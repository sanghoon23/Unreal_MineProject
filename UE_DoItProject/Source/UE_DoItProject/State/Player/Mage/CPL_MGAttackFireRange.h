#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Burn.h"

#include "CPL_MGAttackFireRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MGAttackFireRange 
	: public UCPL_MageBaseAttack, public IIC_Component
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UFUNCTION(Category = "Timer")
		void TimerMontagePause();

	UPROPERTY(VisibleAnywhere, Category = "Actor")
		/* FireRainActor 에서 BeginOverlap 으로 Monster Attacking */
		class ACParticle_FireRain* FireRainActor;

	//// @DamageType
	//UPROPERTY(VisibleAnywhere, Category = "DamageType")
	//	UCDamageType_Normal*	DT_Normal;

	//UPROPERTY(VisibleAnywhere, Category = "DamageType")
	//	UCDamageType_Burn*	DT_Burn;

	#pragma endregion

public:
	UCPL_MGAttackFireRange();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Pure Virtual Function - IC_BaseAttack */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;

	virtual void AttackOtherPawn(UCDamageType_Base* DamageType) override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

private:
	void SetPlayAfterMouseControl();
	void MouseStartWaiting();
	void MouseEndWaiting();


	#pragma region Member
private:
	//@Attack
	float AttackRadius = 250.0f;
	FVector AttackPosition = FVector(0.0f);

	//@MouseControl
	//class UCS_MouseController* MouseController;
	bool bMouseWaiting = false;

	//@Skill Casting
	class UWG_SkillCastingBar* SkillCastWidget;
	bool bFillingReadyGauge = false;
	bool bFillOutGauge = false;
	float FillingReadySpeed = 1.0f;
	float FillOutSpeed = 0.5f;

	//@Monatge
	FTimerHandle MontagePauseTimer;
	float MontagePauseOffset = 2.0f;
	FTimerDelegate MontagePauseDel;

	#pragma endregion

};
