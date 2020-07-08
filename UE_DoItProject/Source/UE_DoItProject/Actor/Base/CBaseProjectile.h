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

protected:
	/* */
	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* ( 전역 ) Projectile 이 따라갈 Target 설정 */
		AActor* SettingTarget = nullptr;

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

	/* Vritual */
public:
	virtual void Explosion() {}

	#pragma region Member 
	/* Member */
public:
	void SettingDirection(FVector Dir);
	void SettingSpeed(float Speed);
	void SettingTargetActor(AActor* Target);


protected:
	float MoveSpeed = 10.0f;
	FVector Direction = FVector(0.0f);

private:
	bool bOverlap = false;


	#pragma endregion
};
