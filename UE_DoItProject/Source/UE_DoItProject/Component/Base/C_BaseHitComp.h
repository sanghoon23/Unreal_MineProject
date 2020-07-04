#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interface/IC_HitComp.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "C_BaseHitComp.generated.h"

UENUM()
enum class EComboOrNot
{
	COMBO = 0,
	NONE = 1,
};

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
	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* HitComp Owner �� �°� �ִ��� - ��Ÿ�� �����, CN_BeginBeat */
		bool bBeated = false;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* �ٸ� ���Ͱ� ���� ��ü�� ���� �� �ִ��� - ������ �Ұ�, ��Ÿ�� �Ұ� */
		bool bCanAttackFromOther = true;

	UPROPERTY(EditAnywhere, Category = "Data")
		/* HitComp �� �ش��ϴ� ��Ÿ�ֵ��� ���� ���� - ������ ����, ��Ÿ�� �Ұ�, */
		bool bBlockDamagedMontage = false;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* �ٸ� ���Ͱ� ���� ��ü���� �������� �� �� �ִ��� ������ �Ұ�, ��Ÿ�� ���� */
		bool bDamaged = true;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* DamageType ���� ���ǵ� Damage Montage */
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> DamagedMontages;

	/*
	���� ���� ���� �޺��� ����� Montage 
	@���� - Montage CNS
	*/
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* HitComboMon;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		TMap<int32, class UMaterialInterface*> Map_OriginPoisionMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		TMap<int32, class UMaterialInterface*> Map_ChangePoisionMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData ���� ����� �Ӹ� �� ǥ���� Particle */
		class UParticleSystem* StunHeadParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData ���� ����� Particle */
		class UParticleSystem* BurnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData ���� ����� Particle */
		class UParticleSystem* FreezeParticle;

protected:
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

	/* Pure Virtual Function - (IC_HitComp ����) */
public:
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount) override;

public:
	virtual const bool IsBeated() const override { return bBeated; }
	virtual void SetBeated(bool bValue) override { bBeated = bValue; }

	virtual UCBaseConditionType* GetConditionData(int Index) override;

	/* IC_HitComp ���� */
	virtual void GetConditionDatasOutArray(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) override;

	/* IC_HitComp ���� */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) override;

	/* IC_HitComp ���� */
	virtual bool IsBlockDamagedMontage() const override { return bBlockDamagedMontage; };
	virtual void SetBlockDamagedMontage(bool bValue) override { bBlockDamagedMontage = bValue; }

	/* IC_HitComp ���� */
	virtual bool IsCanAttackedFromOther() const override { return bCanAttackFromOther; };
	virtual void SetCanAttackedFromOther(bool bValue) override { bCanAttackFromOther = bValue; }

	/* IC_HitComp ���� */
	virtual bool IsDamagedFromOther() const override { return bDamaged; };
	virtual void SetDamagedFromOther(bool bValue) override { bDamaged = bValue; }

	/* IC_HitComp ���� */
	virtual bool IsCanHittedCombo() const override { return bCanHitCombo; };
	virtual void SetCanHittedCombo(bool bValue) { bCanHitCombo = bValue; }

	/* Function */
public:
	void RunMontageFromAttackType(EComboOrNot CanCombo, uint8 TypeNum = 0, float MonSpeed = 0.6f, bool bAlways = true);
	bool AddConditionData(UCBaseConditionType* ConditionData);


	#pragma	region Member
public:
	void GetOriginPoisionMaterialMaps(TMap<int32, class UMaterialInterface*>& Out);
	void GetPoisionMaterialMaps(TMap<int32, class UMaterialInterface*>& Out);
	class UParticleSystem* GetBurnParticleOrNull() const;
	class UParticleSystem* GetFreezeParticleOrNull() const;
	class UParticleSystem* GetStunHeadParticleOrNull() const;

	class UAnimMontage* GetDamagedMontageOrNull(const uint8 ArrayNum);

protected:
	bool bCanHitCombo = false;

private:
	class IIC_Charactor* I_Charactor = nullptr;

	#pragma endregion
};
