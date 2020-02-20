#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CN_EndEquiping.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCN_EndEquiping : public UAnimNotify
{
	GENERATED_BODY()

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
