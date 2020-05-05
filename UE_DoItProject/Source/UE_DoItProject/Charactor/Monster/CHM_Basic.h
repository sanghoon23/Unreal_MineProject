#pragma once

#include "CoreMinimal.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_Charactor.h"

#include "Component/HM_Basic/CHM_BasicAttackComp.h"
#include "Component/HM_Basic/CHM_BasicHitComp.h"
#include "Component/HM_Basic/CHM_BasicEquipComp.h"

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
		class UCHM_BasicHitComp* HitComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_BasicEquipComp* EquipComp;

	#pragma endregion

public:
	ACHM_Basic();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@IC_Charactor
	/* Pure Virtual Function In Interface */
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void CanMove() override { bCanMove = true; }
	virtual void CanNotMove() override { bCanMove = false; }
	virtual void OnGravity() override;
	virtual void OffGravity() override;
	virtual bool IsJumping() override;
	virtual void OffJumping() override {} //@CharactorMovement �� �Ἥ ���� ������������.

	virtual int GetCurrentStateType() const override { return -1; }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;
	virtual void ActorStopCurrentAnimMon() override;
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) override;

	/* Virtual In Interface */
public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; } // @���� �����, Ȥ�� ����� ���� �ִϸ��̼�
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;
	virtual IIC_HitComp* GetIHitComp() override;

public:
	//virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) override;
	//virtual float TakeDamage
	//(float DamageAmount,
	//	struct FDamageEvent const & DamageEvent,
	//	class AController * EventInstigator,
	//	AActor * DamageCauser) override;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@IC_Monster
public:
	virtual FMonsterInfo GetMonsterInfo() const override { return Info; };

	void SetAIRunningPossible(bool bValue) { bAIRunningPossible = bValue; }
	bool GetAIRunningPossible() const { return bAIRunningPossible; }

private:
	void OnDestroy();

	#pragma region Member
private:
	/* IC_Monster Member */
	FMonsterInfo Info;

	#pragma endregion
};
