#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Freeze.h"
#include "Ability/Player/CPLAbility_SpeedDown.h"

#include "CHM_MaoFourAttack.generated.h"

UENUM()
enum class EHM_MaoFourComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	COMBO_TWO = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoFourAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* P_AttackCloud;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Root;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Body;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		TArray<class ACSkillRangeDisplay*> SkillRangeDisplayArray;

	UPROPERTY(EditAnywhere, Category = "AttackData")
		float SkillAttackRangeDisplayHeight = 7.0f;


	UFUNCTION()
	//@Warning - AttackOtherPawn 을 대신할 SkillRangeDisplay 에 Bind 될 함수.
	void DelSkillRangeAttackOtherPawn(AActor* Subject);

	UFUNCTION()
	void DelStartFreezeConditionType(AActor* Subject);

	UFUNCTION()
	void DelEndFreezeConditionType(AActor* Subject);

#pragma endregion

public:
	UCHM_MaoFourAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual Function */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	//virtual bool IsLastCombo() const override;
	//virtual void AttackOtherPawn() override;

private:

#pragma region Member
private:
	class ACHM_PengMao*	HM_PengMao;
	class IIC_Charactor* I_Charactor;

	float StartSectionPlayRate = 0.3f;
	float NextSectionPlayRate = 0.9f;

	float AttackRadius = 1000.0f;

	float AbilityDownSpeedValue = -300.0f;

#pragma endregion

};
