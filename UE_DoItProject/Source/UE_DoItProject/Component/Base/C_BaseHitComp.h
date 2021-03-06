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
	// @Target Info UI 에 보일 TextureData Number
	const int DataNum = 5;

	// @Target Info UI 에 깜빡임을 시작할 시간
	const float OpacityLinearTimer = 3.0f;

	// @Target Info UI 에 깜빡임의 정도
	const float OpacityLinearSpeed = 0.01f;

	#pragma	region Reflection
protected:
	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* HitComp Owner 가 맞고 있는지 - 몽타주 실행시, CN_BeginBeat */
		bool bBeated = false;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* 다른 액터가 현재 객체를 때릴 수 있는지 - 데미지 불가, 몽타주 불가 */
		bool bCanAttackFromOther = true;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* 다른 액터가 현재 객체에게 데미지를 줄 수 있는지 데미지 불가, 몽타주 가능 */
		bool bDamaged = true;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* HitComp 에 해당하는 몽타주들의 실행 여부 - 데미지 가능, 몽타주 불가, */
		TArray<bool> bUsingDamageTypeEffect;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* DamageType 별로 정의된 Damage Montage */
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> DamagedMontages;

	/*
	맞은 상태 에서 콤보가 실행될 Montage 
	@조건 - Montage CNS
	*/
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* HitComboMon;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TArray<class USkeletalMesh*> CharactorMeshArray;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData 에서 사용할 머리 위 표시할 Particle */
		class UParticleSystem* StunHeadParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData 에서 사용할 Particle */
		class UParticleSystem* BurnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData 에서 사용할 Particle */
		class UParticleSystem* FreezeParticle;

protected:
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

	/* Pure Virtual Function - (IC_HitComp 참고) */
public:
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount) override;

	/* TArray<class USkeletalMesh*> 에 등록된 Skeletal Mesh 로 전환 */
	virtual void SettingCustomCharactorMesh(ECharactorMeshSort MeshSort, bool bNoneRestartAnimation = false) override;

	/* Hit 당했을 때, 때린 객체 - (이전에 때린 객체가 될 수 있음) */
	virtual const AActor* GetAttacker() const override { return Attacker; }

public:
	/* IC_HitComp 참조 */
	virtual bool IsEmptyConditionArray() const override;

	/* IC_HitComp 참조 */
	virtual UCBaseConditionType* GetConditionData(int Index) override;

	/* IC_HitComp 참조 */
	virtual void GetConditionDatasOutArray(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) override;

	/* IC_HitComp 참조 */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) override;

	/* IC_HitComp 참조 */
	virtual bool GetApplyUpsetStateInContainer(EHitUpset Sort, int& IndexCount) override;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@Member

	virtual const bool IsBeated() const override { return bBeated; }
	virtual void SetBeated(bool bValue) override { bBeated = bValue; }

	//* IC_HitComp 참조 */
	virtual bool IsUsingDamageTypeEffect(uint8 Type) const override { return bUsingDamageTypeEffect[Type]; };

	/* IC_HitComp 참조 */
	virtual bool IsCanAttackedFromOther() const override { return bCanAttackFromOther; };
	virtual void SetCanAttackedFromOther(bool bValue) override { bCanAttackFromOther = bValue; }

	/* IC_HitComp 참조 */
	virtual bool IsDamagedFromOther() const override { return bDamaged; };
	virtual void SetDamagedFromOther(bool bValue) override { bDamaged = bValue; }

	/* IC_HitComp 참조 */
	virtual bool IsCanHittedCombo() const override { return bCanHitCombo; };
	virtual void SetCanHittedCombo(bool bValue) { bCanHitCombo = bValue; }

	/* Hit 당했을 때, 움직일 '방향' */
	virtual FVector GetHitDirection() const override { return HitDirection; }
	virtual void SetHitDirection(FVector Direction) override { HitDirection = Direction; }

	/* Hit 당했을 때, 움직일 '속도' */
	virtual float GetHitMoveSpeed() const override { return HitMoveSpeed; }
	virtual void SetHitMoveSpeed(float fValue) override { HitMoveSpeed = fValue; }

	/* Function */
public:
	void RunMontageFromAttackType(EComboOrNot CanCombo, uint8 TypeNum = 0, float MonSpeed = 0.6f, bool bAlways = true);
	bool AddConditionData(UCBaseConditionType* ConditionData);


	#pragma	region Member
public:
	class UParticleSystem* GetBurnParticleOrNull() const;
	class UParticleSystem* GetFreezeParticleOrNull() const;
	class UParticleSystem* GetStunHeadParticleOrNull() const;

	class UAnimMontage* GetDamagedMontageOrNull(const uint8 ArrayNum);

protected:
	bool bCanHitCombo = false;

private:
	class IIC_Charactor* I_Charactor = nullptr;

	FVector HitDirection = FVector(0.0f);
	float HitMoveSpeed = 1.0f; //@Charactor Movement 를 사용. (Default=1.0f)

	/* 때린 객체 */
	AActor* Attacker = nullptr;

	#pragma endregion
};
