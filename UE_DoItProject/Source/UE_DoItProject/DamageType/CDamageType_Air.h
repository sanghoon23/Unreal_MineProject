#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Air.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Air 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCDamageType_Air();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;
};
