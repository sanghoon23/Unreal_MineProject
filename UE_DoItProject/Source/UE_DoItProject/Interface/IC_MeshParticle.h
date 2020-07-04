#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_MeshParticle.generated.h"

UENUM()
enum class EAttachPointRelative
{
	NONE = 0,
	RELATIVE = 1,
};

UENUM()
enum class EAttachPointType
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
	virtual class UParticleSystemComponent* SpawnParticleAtMesh(class UParticleSystem* const PT, const EAttachPointType Point, const EAttachPointRelative Rel, const EAttachLocation::Type AttachLocation) = 0;
	virtual class UParticleSystemComponent* SpawnParticleAtMesh(class UParticleSystem* const PT, const EAttachPointType Point, const EAttachPointRelative Rel, const EAttachLocation::Type AttachLocation, FTransform Transform) = 0;
	virtual bool SetLocationParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel) = 0;
	virtual void AttachParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel) = 0;
};
