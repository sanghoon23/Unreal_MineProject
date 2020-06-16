#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CCharactorMovementComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCCharactorMovementComp 
	: public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AIControl")
		bool bSliding = false;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};
