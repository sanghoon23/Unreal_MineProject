#pragma once

#include "CoreMinimal.h"
#include "Charactor/Monster/Base/CMonsterHumanoid.h"
#include "Interface/IC_Charactor.h"

#include "CMH_Basic.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACMH_Basic 
	: public ACMonsterHumanoid, public IIC_Charactor
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	ACMH_Basic();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void CanMove() override { bCanMove = true; }
	virtual void CanNotMove() override { bCanMove = false; }
	virtual int GetCurrentStateType() const override { return -1; }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;


	// virtual IIC_AttackComp* GetIAttackComp() override;

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
	bool bCanMove = true;

	#pragma endregion
};
