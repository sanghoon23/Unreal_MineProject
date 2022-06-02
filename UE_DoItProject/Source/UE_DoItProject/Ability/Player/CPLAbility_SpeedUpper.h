#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_SpeedUpper.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_SpeedUpper
	: public UCBaseAbility
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* FasterParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_LHand;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_RHand;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		UParticleSystemComponent* PTComp_LHand;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		UParticleSystemComponent* PTComp_RHand;

	#pragma endregion

public:
	UCPLAbility_SpeedUpper();

	/* Virtual Function */
public:
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() override;

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	//virtual void ApplyAbility();

};
