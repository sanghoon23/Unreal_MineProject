#pragma once

#include "CoreMinimal.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_Charactor.h"

#include "Component/HM_Shaman/CHM_ShamanAttackComp.h"
#include "Component/HM_Shaman/CHM_ShamanEquipComp.h"
#include "Component/HM_Shaman/CHM_ShamanHitComp.h"
#include "Component/CMeshParticleComp.h"

#include "CHM_Shaman.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_Shaman 
	: public ACHumanoidMonster, public IIC_Monster, public IIC_Charactor
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		bool bAIRunningPossible = true;

	UPROPERTY(EditAnywhere, Category = "AI")
		bool bAIAttackMode = false;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_ShamanAttackComp* AttackComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_ShamanEquipComp* EquipComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_ShamanHitComp* HitComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCMeshParticleComp* MeshParticleComponent;

	#pragma endregion

public:
	ACHM_Shaman();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Pure Virtual Function In Interface - (IC_Charactor) */
public:
	virtual ECollisionChannel GetCharactorUsingChannel() const override
	{
		return ECollisionChannel::ECC_GameTraceChannel3; //@Monster Attack
	}
	virtual ECharactorType GetCharactorType() const override { return CharactorType; }

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
	//@Virtual Pure Function - (IC_Monster)
public:
	virtual AActor* GetTargetInAI() override;

	virtual const FMonsterInfo& GetMonsterInfo() const override { return Info; };

	void SetAIRunningPossible(bool bValue) { bAIRunningPossible = bValue; }
	bool GetAIRunningPossible() const { return bAIRunningPossible; }

	void SetAIAttackMode(bool bValue);
	virtual bool GetAIAttackMode() const { return bAIAttackMode; }

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
	// Type
	ECharactorType CharactorType = ECharactorType::MONSTER;

	/* IC_Monster Member */
	FMonsterInfo Info;

	#pragma endregion

};
