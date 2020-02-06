#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CN_ItemDetach.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCN_ItemDetach : public UAnimNotify
{
	GENERATED_BODY()

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
