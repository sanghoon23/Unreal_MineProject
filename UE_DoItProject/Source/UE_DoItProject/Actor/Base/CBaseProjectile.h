#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

USTRUCT()
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

public:
	FProjectileData() {};

public:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	float MoveSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	FVector Direction = FVector(0.0f);

	UPROPERTY(VisibleAnywhere, Category = "Data")
	AActor* FollowingTarget = nullptr;
};

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
		AActor* FollowingTarget = nullptr;

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

	/* Function */
public:
	/* Montage 에서 Projectile 을 Spawn 하기 위해서 static 설정 */
	static void SettingProjectile(FVector Dir, float Speed = 10.0f);

	/* Montage 에서 Projectile 을 Spawn 하기 위해서 static 설정 */
	static void SettingProjectile(AActor* FollowingActor, FVector Dir, float Speed = 10.0f);

	/* Montage 에서 Projectile 을 Spawn 하기 위해서 static 설정 */
	static void SettingProjectile(const FProjectileData& Data);

	#pragma region Member 
	/* Member */
public:


protected:
	static float MoveSpeed;
	static FVector Direction;

	/* ( 전역 ) Projectile 이 따라갈 Target 설정 */
	static AActor* SettingTarget;

private:
	bool bOverlap = false;

	#pragma endregion
};
