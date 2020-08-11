#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "CDamageType_Base.generated.h"

UENUM()
enum class FDamageType
{
	NORMAL			= 0,
	AIR				= 1,
	AIRATTACK		= 2,
	STRONGATTACK	= 3,
	STUN			= 4,
	BURN			= 5,
	POISION			= 6,
	FREEZE			= 7,
	SLOWER			= 8,
	END				= 9,
};

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Base
	: public UDamageType
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:

protected:
	UPROPERTY(EditAnywhere, Category = "DamageType")
		int TypeNumber = 0;

	UPROPERTY(EditAnywhere, Category = "DamageType")
		FDamageType DamageType;

	UPROPERTY(EditAnywhere, Category = "Data")
		float HitMoveSpeed = 0.0f;

	#pragma endregion

public:
	UCDamageType_Base();

	/* Virtual */
public:
	/*
	If 를 줄이기 위해 자체적으로
	DamageType 에서 ConditionType -> IHitComp->AddConditionData 로 넘어가기 위한 수단의 함수
	*/
	//@param Subject - 주체자
	//@param DamagedActor - 맞는 액터
	//@param InitialDamageAmount - 초기에 들어갈 데미지 값
	//@param Montage - 맞는 액터가 시행할 Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	#pragma region Member
	/* Virutla Member */
public:
	virtual void SetDamageImpulse(float DamageAmount);

	virtual void SetHitMoveSpeed(float fSpeed);
	const float GetHitMoveSpeed() const { return HitMoveSpeed; }

	virtual void SetDestructibleImpulse(float DestructibleDamageAmount);

	virtual void SetDestructibleDamageSpreadScale(float SpreadScale);

public:
	int GetTypeNumber() const { return TypeNumber; }
	FDamageType GetConditionType() const { return DamageType; }

protected:

	#pragma endregion
};
