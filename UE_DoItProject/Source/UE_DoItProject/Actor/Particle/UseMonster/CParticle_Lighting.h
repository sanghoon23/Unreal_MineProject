#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Stun.h"

#include "CParticle_Lighting.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACParticle_Lighting 
	: public AActor
{
	GENERATED_BODY()
	
private:
	//const float AttackRepeatTime = 1.0f;
	const float ActorOverlapSphereRadius = 200.0f;

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;

	/* BeginOverlap ���� ���� DamageType */
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Base* DT_UseBeginOverlap;

	/* �ٸ� ���Ϳ� ��ħ�� �Ͼ�� ��, - AddDynamic */
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* �ٸ� ���Ϳ� ��ħ�� ������ ��, - AddDynamic */
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
	//	/* �ٸ� ���Ϳ� ��� ��ħ�� ��, */
	//	void OnAttackingOverlap();

	#pragma endregion

	
public:	
	ACParticle_Lighting();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void OnStartActor(FVector Position);
	void OffEndActor();

public:
	void SetDamageType(UCDamageType_Base* UseBeginOverlap);

	/* FVector(fValue, fValue, 32.0f) - Z �� 32.0f �� ���� */
	void SetBoxExtentScale(float fValue);

	/* FVector(Vec.X, Vec.Y, 32.0f) - Z �� 32.0f �� ���� */
	void SetBoxExtentScale(FVector VecScale);

	void SetParticleCompRelative(FTransform Transform);

	/* Member */
private:
	float TickTimer = 0.0f;
	bool bCollisioning = false;

};
