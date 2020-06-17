#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"

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
	END				= 8,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDamageFunc, AActor*)

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Base
	: public UDamageType
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	//UFUNCTION(BlueprintCallable, Category = "Texture")
	//	virtual class UTexture2D* GetDamageTypeTexture() {}

protected:
	UPROPERTY(EditAnywhere, Category = "DamageType")
		int TypeNumber = 0;

	UPROPERTY(EditAnywhere, Category = "DamageType")
		FDamageType DamageType;

	#pragma endregion

public:
	UCDamageType_Base();

	/* Virtual */
public:
	/*
	If �� ���̱� ���� ��ü������
	DamageType ���� ConditionType -> IHitComp->AddConditionData �� �Ѿ�� ���� ������ �Լ�
	*/
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	// @DamageFunc Delegate ���� 
	// @param Actor - ���ݴ��ϴ�, ������ �޴� Actor
	virtual void OnDamageDelegate(AActor* DamagedActor);

	#pragma region Member
	/* Virutla Member */
public:
	virtual void SetDamageImpulse(float DamageAmount);
	virtual void SetDestructibleImpulse(float DestructibleDamageAmount);
	virtual void SetDestructibleDamageSpreadScale(float SpreadScale);

public:
	FDamageFunc& GetDamageDelegate() { return DamageFunc; }

	int GetTypeNumber() const { return TypeNumber; }
	FDamageType GetConditionType() const { return DamageType; }

protected:
	/* @HitComp �� ���� ��, ������ Delegate */
	FDamageFunc DamageFunc;

	#pragma endregion
};
