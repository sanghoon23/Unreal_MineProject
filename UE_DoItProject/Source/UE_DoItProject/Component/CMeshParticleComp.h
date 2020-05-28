#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_MeshParticle.h"

#include "CMeshParticleComp.generated.h"

//USTRUCT()
//struct FMeshParticle
//{
//	GENERATED_USTRUCT_BODY()
//
//	//AttachPointType Type = EAttachPointType::ROOT;
//	FName TypeName = "";
//
//	UPROPERTY(VisibleAnywhere, Category = "Component")
//		class UParticleSystemComponent* ParticleComp;
//};

UCLASS()
class UE_DOITPROJECT_API UCMeshParticleComp 
	: public UActorComponent, public IIC_MeshParticle
{
	GENERATED_BODY()

	/*
	@Warning - 
	Relative 를 쓰기 위해선 Name 앞에 Relative를 붙어야 함.
	*/

	#pragma	region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Attach")
		FName RelativeTypeName = "Relative";

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachRootName = "SocketRoot";

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachHeadName = "SocketHead";

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachBodyName = "SocketBody";

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachLeftHandName = "SocketHand_L";

	UPROPERTY(EditAnywhere, Category = "Attach")
		FName AttachRightHandName = "SocketHand_R";

	//UPROPERTY(VisibleAnywhere, Category = "Container")
	//	TArray<struct FMeshParticle> ApplyParticles;

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
	//void AddParticleCompAtContainer(class UParticleSystemComponent* const PTComp, const EAttachPointTypePoint) override;

	class UParticleSystemComponent* SpawnParticleAtMesh(class UParticleSystem* const PT, const EAttachPointType Point, const EAttachPointRelative Rel, const EAttachLocation::Type AttachLocation) override;
	bool SetLocationParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel) override;
	void AttachParticleCompAtMesh(class UParticleSystemComponent* const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel) override;

private:
	bool CheckAttackName(FName& OutName, const EAttachPointType Point, const EAttachPointRelative Rel);

	/* Member */
private:
	class ACharacter* CharactorOwner;
		
};
