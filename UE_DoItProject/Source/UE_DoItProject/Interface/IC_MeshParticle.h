#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_MeshParticle.generated.h"

UENUM()
enum class AttachPointType
{
	ROOT = 0,
	HEAD = 1,
	BODY = 2,
	LHAND = 3,
	RHAND = 4,
};

UINTERFACE(MinimalAPI)
class UIC_MeshParticle : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_MeshParticle
{
	GENERATED_BODY()
	
	/* Pure Virtual Function */
public:
	virtual class UParticleSystemComponent* SpawnParticleAtMesh(class UParticleSystem* const PT, const AttachPointType Point, const EAttachLocation::Type AttachLocation) = 0;
	virtual bool SetLocationParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const AttachPointType Point) = 0;
	virtual void AttachParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const AttachPointType Point) = 0;
};
