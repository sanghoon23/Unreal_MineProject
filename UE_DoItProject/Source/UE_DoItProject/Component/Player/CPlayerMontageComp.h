#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_MontageComp.h"

#include "CPlayerMontageComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPlayerMontageComp 
	: public UActorComponent, public IIC_MontageComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPlayerMontageComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// virtual void PlayAnimation(UINT MonType, UINT PlayAnim, float Speed, bool bAlways) override;


private:
	// void AttackMontageAdd(UAttackMontageType MonType, class UAnimMontage* Montage);

#pragma	region Member
private:
	// TMap<UAttackMontageType, TArray<class UAnimMontage*> > AnimMonMap;
#pragma endregion
};
