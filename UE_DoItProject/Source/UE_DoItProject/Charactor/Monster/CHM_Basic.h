#pragma once

#include "CoreMinimal.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_Charactor.h"

#include "Component/HM_Basic/CHM_BasicAttackComp.h"
#include "Component/HM_Basic/CHM_BasicHitComp.h"
#include "Component/HM_Basic/CHM_BasicEquipComp.h"
#include "Component/CMeshParticleComp.h"


#include "CHM_Basic.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_Basic
	: public ACHumanoidMonster, public IIC_Monster, public IIC_Charactor
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		bool bAIRunningPossible = true;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_BasicAttackComp* AttackComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_BasicHitComp* HitComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_BasicEquipComp* EquipComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCMeshParticleComp* MeshParticleComp;

	#pragma endregion

public:
	ACHM_Basic();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Pure Virtual Function In Interface - (IC_Charactor) */
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void OnDeath() override; //죽음
	virtual void CanMove() override { bCanMove = true; }
	virtual void CanNotMove() override { bCanMove = false; }
	virtual void OnGravity() override;
	virtual void OffGravity() override;
	virtual bool IsJumping() override;
	virtual void OffJumping() override {} //@CharactorMovement 를 써서 따로 정의하지않음.

	virtual int GetCurrentStateType() const override { return -1; }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;
	virtual void ActorStopCurrentAnimMon() override;
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) override;
	virtual void ActorPausedAnimMonResume() override;
	virtual void ActorAnimMonPause() override;

	/* Virtual In Interface - (IC_Charactor) */
public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; } // @현재 적용된, 혹은 적용된 후의 애니메이션
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;
	virtual IIC_HitComp* GetIHitComp() override;
	virtual IIC_MeshParticle* GetIMeshParticle() override;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@IC_Monster
public:
	virtual const FMonsterInfo& GetMonsterInfo() const override { return Info; };

	void SetAIRunningPossible(bool bValue) { bAIRunningPossible = bValue; }
	bool GetAIRunningPossible() const { return bAIRunningPossible; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Class Virtual */
public:
	//@return float - 현재 남은 체력 - (TargetInfo.CurrentHP)
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	
	virtual void OnCollision() override;
	virtual void OffCollision() override;

private:
	void OnDelegateCharactorDestroy();
	void CallDestory();

	#pragma region Member
private:
	/* IC_Monster Member */
	FMonsterInfo Info;

	#pragma endregion
};
