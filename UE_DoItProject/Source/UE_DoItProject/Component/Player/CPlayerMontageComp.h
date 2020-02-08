#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_MontageComp.h"
#include "Component/Player/CPL_MageAttackComp.h"
#include "Component/Player/CPL_SwordAttackComp.h"

#include "CPlayerMontageComp.generated.h"

UENUM()
enum class MontageSort : uint8 // @MontageSort - 해당 Actor 의 전체 몽타주 종류
{
	Common = 0,
	Move = 1,
	Mage = 2,
	Sword = 3,
	End = 4,
};

UENUM()
enum class CommonMontageType : uint8 // @CommonMontageType - 공통적으로 쓰일 몽타주
{
	END = 0,
};

UENUM()
enum class MoveMontageType : uint8 // @MoveMontageType - 이동 관련 몽타주
{
	EVADE_F = 0,
	EVADE_B = 1,
	EVADE_L = 2,
	EVADE_R = 3,
	END		= 4,
};

UCLASS()
class UE_DOITPROJECT_API UCPlayerMontageComp 
	: public UActorComponent, public IIC_MontageComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> CommonMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> MoveMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> MageAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> SwordAttackMontages;

	#pragma endregion

public:	
	UCPlayerMontageComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void PlayAnimation
	(
		UINT MontageType, UINT PlayMontageNum,
		UINT Sort, float Speed = 1.5f, bool bAlways = false
	) override;


private:
	void MontageAddInMap(UINT MontageType, UINT Sort, class UAnimMontage* Montage);

	#pragma	region Member
private:
	TMap<MageAttackType, TArray<class UAnimMontage*> > MageMontageMap;
	TMap<SwordAttackType, TArray<class UAnimMontage*> > SwordMontageMap;
	#pragma endregion
};
