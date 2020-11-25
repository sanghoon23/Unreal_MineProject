#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLockWall_ForNextStage.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACLockWall_ForNextStage
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#pragma endregion

	
public:	
	ACLockWall_ForNextStage();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

};
