#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CN_SetCanAttackedFromOther.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCN_SetCanAttackedFromOther
	: public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Value")
		bool bCanAttackFromOther = false;

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
