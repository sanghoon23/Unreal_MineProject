#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_MeshParticle.h"

#include "CMeshParticleComp.generated.h"

USTRUCT()
struct FMeshParticle
{
	GENERATED_USTRUCT_BODY()

	//AttachPointType Type = AttachPointType::ROOT;
	FName TypeName = "";

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;
};

UCLASS()
class UE_DOITPROJECT_API UCMeshParticleComp 
	: public UActorComponent, public IIC_MeshParticle
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachRootName;

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachHeadName;

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachBodyName;

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachLeftHandName;

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachRightHandName;

	UPROPERTY(VisibleAnywhere, Category = "Container")
		TArray<struct FMeshParticle> ApplyParticles;

private:

	#pragma endregion

public:	
	UCMeshParticleComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	//void AddParticleCompAtContainer(class UParticleSystemComponent* const PTComp, const AttachPointType Point) override;

	void SpawnParticleAtMesh(class UParticleSystem* const PT, const AttachPointType Point, const EAttachLocation::Type AttachLocation) override;
	bool SetLocationParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const AttachPointType Point) override;

private:
	bool CheckAttackName(FName& OutName, const AttachPointType Point);

	/* Member */
private:
	class ACharacter* CharactorOwner;
		
};
