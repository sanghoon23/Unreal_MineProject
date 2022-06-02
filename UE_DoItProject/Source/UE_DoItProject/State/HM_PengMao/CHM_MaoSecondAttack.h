#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_StrongAttack.h"
#include "DamageType/CDamageType_Freeze.h"

#include "Ability/Player/CPLAbility_SpeedDown.h"

#include "CHM_MaoSecondAttack.generated.h"

UENUM()
enum class EHM_MaoSecondComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	COMBO_TWO = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoSecondAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Freeze*	DT_Freeze;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Root;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Body;


public:
	UFUNCTION()
		void TimerFreezeHittedActor(AActor* Subject, UCDamageType_Freeze* Param_Freeze);

#pragma endregion

public:
	UCHM_MaoSecondAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual Function */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual bool IsLastCombo() const override;

	virtual void AttackOtherPawn(UCDamageType_Base* DamageType) override;

	/* Function */
private:
	void BeginBeatedFunction(AActor* Subject);
	void EndBeatedFunction(AActor* Subject);

#pragma region Member
private:
	class ACHM_PengMao*	HM_PengMao;
	class IIC_Charactor* I_Charactor;

	float AttackRange = 200.0f;
	float AttackRadius = 400.0f;

	float OffSetAttackRangeForStart = 200.0f; //@Collision ������(ActorLocation) �� Offset �չ���
	TArray<float> AttackRangeVec; //@���� �Ÿ�(Sphere �� ������ ����)
	TArray<float> AttackRadiusVec; //@���� �ѷ� - Sphere

	float AbilityDownSpeedValue = -300.0f;

	FTimerDelegate FreezeTimerDelegate;

#pragma endregion

};
