#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CN_EndEvade.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCN_EndEvade : public UAnimNotify
{
	GENERATED_BODY()

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
