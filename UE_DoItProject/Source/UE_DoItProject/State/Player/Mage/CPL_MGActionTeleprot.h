#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_MGActionTeleprot.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MGActionTeleprot
	: public UC_BaseActionState
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* TeleprotMontage;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* TeleportParticle;

	UFUNCTION(Category = "Func")
		void PlayAfterMouseControl();

	UFUNCTION(Category = "Func")
		void PlayerVisibleAfterParticle();

	#pragma endregion

public:
	UCPL_MGActionTeleprot();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - Interface */
public:
	void OnAction() override;

	/* Function */
private:
	void SetPlayAfterMouseControl();
	void StartWaiting();
	void EndWaiting();

	/* Member */
private:
	class ACPlayer* Player;
	class UCS_MouseController* MouseController;

	bool bStartWaiting = false;

	FTimerHandle EndMontageTimerHandle;
	FTimerDelegate PlayHandle;
};
