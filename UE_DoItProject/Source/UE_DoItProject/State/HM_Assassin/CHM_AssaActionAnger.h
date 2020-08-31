#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"


#include "CHM_AssaActionAnger.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_AssaActionAnger 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystem* P_Action;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystemComponent* PT_Background;

#pragma endregion

public:
	UCHM_AssaActionAnger();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void OnAction() override;

public:
	virtual void BeginActionState() override;
	virtual void TickActionState() override;
	virtual void EndActionState() override;

	/* Member */
private:
	class ACHM_Assassin* HM_Assassin;

};
