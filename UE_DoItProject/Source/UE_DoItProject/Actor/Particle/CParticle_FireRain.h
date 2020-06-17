#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DamageType/Base/CDamageType_Base.h"

#include "CParticle_FireRain.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACParticle_FireRain 
	: public AActor
{
	GENERATED_BODY()
	
private:
	const float AttackRepeatTime = 1.0f;
	const float ActorOverlapSphereRadius = 200.0f;

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;

	/* BeginOverlap 에서 쓰일 DamageType */
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Base* DT_UseBeginOverlap;

	/* 객체가 유지될 때, TraceChannel Collision 에 사용될 DamageType */
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Base*	DT_DT_UseAttackRepeat;

	/* 다른 액터와 겹침이 일어났을 때, - AddDynamic */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* 다른 액터와 겹침이 끝났을 때, - AddDynamic */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	/* 다른 액터와 계속 겹침될 때, */
	void OnAttackingOverlap();

	#pragma endregion

	
public:	
	ACParticle_FireRain();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void OnStartActor(FVector Position);
	void OffEndActor();

public:
	void SetDamageType(UCDamageType_Base* UseBeginOverlap, UCDamageType_Base* UseAttackRepeat);

	/* FVector(fValue, fValue, 32.0f) - Z 값 32.0f 로 고정 */
	void SetBoxExtentScale(float fValue);

	/* FVector(Vec.X, Vec.Y, 32.0f) - Z 값 32.0f 로 고정 */
	void SetBoxExtentScale(FVector VecScale);

	/* ParticleComp */
	void SetParticleCompRelative(FTransform Transform);

	/* Member */
private:
	float TickTimer = 0.0f;
	bool bCollisioning = false;

};
