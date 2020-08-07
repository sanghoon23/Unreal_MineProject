#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "Ability/Player/CPLAbility_SpeedDown.h"

#include "CHM_MaoFirstAttack.generated.h"

UENUM()
enum class EHM_MaoFirstComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	COMBO_TWO = 2,
	COMBO_THREE = 3,
	COMBO_FOUR = 4,
	END = 5,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoFirstAttack
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Noraml;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_Strong;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Root;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Body;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
		class UCPLAbility_SpeedDown* AbilitySpeedDowner;

#pragma endregion

public:
	UCHM_MaoFirstAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	//
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual bool IsLastCombo() const override;

	virtual void AttackOtherPawn() override;

private:
	//@Delegate - HitComp
	void BeginBeatedFunction(AActor* Subject);
	void EndBeatedFunction(AActor* Subject);

#pragma region Member
private:
	class ACHM_PengMao*	HM_PengMao;
	class IIC_Charactor* I_Charactor;

	float OffSetAttackRangeForStart = 190.0f; //@Collision 시작점(ActorLocation) 의 Offset 앞방향
	TArray<float> AttackRangeVec; //@공격 거리(Sphere 가 움직일 범위)
	TArray<float> AttackRadiusVec; //@공격 둘레 - Sphere

	float AbilityDownSpeedValue = -300.0f;

#pragma endregion

};
