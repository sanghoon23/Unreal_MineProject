#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACBaseProjectile 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
public:
	UFUNCTION(BlueprintCallable)
		virtual void Death();

	UFUNCTION(BlueprintCallable)
		static void SettingProjectile(FVector Dir, float Speed = 10.0f);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* StaticMesh;

	/* Virtual Overlap */
	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* Virtual Overlap */
	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	#pragma endregion
	
public:	
	ACBaseProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:

	#pragma region Member 

	/* Member */
protected:
	static float MoveSpeed;
	static FVector Direction;

private:
	bool bOverlap = false;

	#pragma endregion
};
