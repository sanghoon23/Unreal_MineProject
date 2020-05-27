#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_Speed.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_Speed 
	: public UCBaseAbility
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_LHand;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_RHand;

	UPROPERTY(VisibleAnywhere, Category = "ReturnValue")
		class UParticleSystemComponent* ParticleComp_LHand;

	UPROPERTY(VisibleAnywhere, Category = "ReturnValue")
		class UParticleSystemComponent* ParticleComp_RHand;

	#pragma endregion

public:
	UCPLAbility_Speed();

	/* Virtual Function */
public:
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() override;

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() override;

	virtual bool OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	//virtual void ApplyAbility();

};
