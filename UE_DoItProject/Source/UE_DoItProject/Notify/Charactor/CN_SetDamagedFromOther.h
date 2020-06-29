#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CN_SetDamagedFromOther.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCN_SetDamagedFromOther 
	: public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Value")
		bool bDamagedFromOther = false;

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
