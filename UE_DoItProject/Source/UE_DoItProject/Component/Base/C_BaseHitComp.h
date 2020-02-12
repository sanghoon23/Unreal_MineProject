#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_HitComp.h"

#include "C_BaseHitComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UC_BaseHitComp 
	: public UActorComponent, public IIC_HitComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> HitMontages;

	#pragma endregion

public:	
	UC_BaseHitComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

	#pragma	region Member
private:

	#pragma endregion
};
