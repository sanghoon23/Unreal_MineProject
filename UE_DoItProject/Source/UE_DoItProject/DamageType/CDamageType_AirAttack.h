#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_AirAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_AirAttack 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCDamageType_AirAttack();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;
};
