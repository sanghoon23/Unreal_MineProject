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
	// @Target Info UI 에 보일 TextureData Number
	const int DataNum = 5;

	// @Target Info UI 에 깜빡임을 시작할 시간
	const float OpacityLinearTimer = 3.0f;

	// @Target Info UI 에 깜빡임의 정도
	const float OpacityLinearSpeed = 0.01f;

	#pragma	region Reflection
protected:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		/* HitComp 에 해당하는 몽타주들의 실행 여부 */
		bool bBlockDamagedMontage = false;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> DamagedMontages;

	UPROPERTY(VisibleAnywhere, Category = "ConditionData")
		/* Poision ConditionData 에서 사용할 Material */
		class UMaterialInterface* PoisionMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData 에서 사용할 머리 위 표시할 Particle */
		class UParticleSystem* StunHeadParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData 에서 사용할 Particle */
		class UParticleSystem* BurnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData 에서 사용할 Particle */
		class UParticleSystem* FreezeParticle;

private:
	/*
	@상태이상, 행동불가 를 다음 ConditionData TArray 
	* UHitUpsetConditionData(상태이상) , * UHitNonActionConditionData(행동불가)
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

	/* IC_HitComp 참조 */
	virtual void GetConditionDatasAfterEmpty(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) override;

	/* IC_HitComp 참조 */
	virtual void GetConditionDatasByCount(TArray<UCBaseConditionType*>* OutDataArray, int IndexCount) override;

	/* IC_HitComp 참조 */
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
