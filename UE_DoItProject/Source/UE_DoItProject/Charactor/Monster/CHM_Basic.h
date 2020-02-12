#pragma once

#include "CoreMinimal.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Interface/IC_Charactor.h"

#include "Component/HM_Basic/CHM_BasicAttackComp.h"
#include "Component/HM_Basic/CHM_BasicEquipComp.h"

#include "CHM_Basic.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHM_Basic
	: public ACHumanoidMonster, public IIC_Charactor
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCHM_BasicAttackComp* AttackComp;

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
	/* Pure Virtual Function In Interface */
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void CanMove() override { bCanMove = true; }
	virtual void CanNotMove() override { bCanMove = false; }
	virtual int GetCurrentStateType() const override { return -1; }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;

	/* Virtual In Interface */
public:
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;

public:
	//virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) override;
	//virtual float TakeDamage
	//(float DamageAmount,
	//	struct FDamageEvent const & DamageEvent,
	//	class AController * EventInstigator,
	//	AActor * DamageCauser) override;

private:
	void OnDestroy();

	#pragma region Member
private:

	#pragma endregion
};
