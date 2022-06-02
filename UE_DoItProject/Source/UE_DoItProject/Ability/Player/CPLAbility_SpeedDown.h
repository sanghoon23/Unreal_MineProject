#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_SpeedDown.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_SpeedDown 
	: public UCBaseAbility
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Root;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SlowerParticle_Body;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		UParticleSystemComponent* PTComp_SlowerRoot;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		UParticleSystemComponent* PTComp_SlowerBody;

#pragma endregion

public:
	UCPLAbility_SpeedDown();

	/* Virtual Function */
public:
	/* Timer 를 사용한 능력치 적용 시작 */
	virtual void StartUseTimerAbility() override;

	/* Timer 를 사용한 능력치 적용 종료 */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

};
