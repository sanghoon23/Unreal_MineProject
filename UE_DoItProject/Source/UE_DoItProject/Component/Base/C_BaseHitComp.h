#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interface/IC_HitComp.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "C_BaseHitComp.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @HitComponent

UCLASS( ClassGroup=(Custom) )
class UE_DOITPROJECT_API UC_BaseHitComp 
	: public UActorComponent, public IIC_HitComp
{
	GENERATED_BODY()

private:
	// @Target Info UI �� ���� TextureData Number
	const int DataNum = 5;

	// @Target Info UI �� �������� ������ �ð�
	const float OpacityLinearTimer = 3.0f;

	// @Target Info UI �� �������� ����
	const float OpacityLinearSpeed = 0.01f;

	#pragma	region Reflection
protected:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		/* HitComp �� �ش��ϴ� ��Ÿ�ֵ��� ���� ���� */
		bool bBlockDamagedMontage = false;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> DamagedMontages;

	UPROPERTY(VisibleAnywhere, Category = "ConditionData")
		/* Poision ConditionData ���� ����� Material */
		class UMaterialInterface* PoisionMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData ���� ����� �Ӹ� �� ǥ���� Particle */
		class UParticleSystem* StunHeadParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData ���� ����� Particle */
		class UParticleSystem* BurnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData ���� ����� Particle */
		class UParticleSystem* FreezeParticle;

private:
	/*
	@�����̻�, �ൿ�Ұ� �� ���� ConditionData TArray 
	* UHitUpsetConditionData(�����̻�) , * UHitNonActionConditionData(�ൿ�Ұ�)
	*/
	UPROPERTY(VisibleAnywhere, Category = "ConditionData")
	TArray<UCBaseConditionType*> ConditionDatas;

	#pragma endregion

public:	
	UC_BaseHitComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function */
public:
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount) override {}

	virtual UCBaseConditionType* GetConditionData(int Index) override;

	/* IC_HitComp ���� */
	virtual void GetConditionDatasAfterEmpty(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) override;

	/* IC_HitComp ���� */
	virtual void GetConditionDatasByCount(TArray<UCBaseConditionType*>* OutDataArray, int IndexCount) override;

	/* IC_HitComp ���� */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) override;


	/* Function */
public:
	bool AddConditionData(UCBaseConditionType* ConditionData);

	#pragma	region Member
public:
	class UMaterialInterface* GetPoisionMaterialOrNull() const;
	class UParticleSystem* GetBurnParticleOrNull() const;
	class UParticleSystem* GetFreezeParticleOrNull() const;
	class UParticleSystem* GetStunHeadParticleOrNull() const;

	class UAnimMontage* GetDamagedMontageOrNull(const uint8 ArrayNum);

	virtual void SetBlockDamagedMontage(bool bValue) override { bBlockDamagedMontage = bValue; }

private:

	#pragma endregion
};
