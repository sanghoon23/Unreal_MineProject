#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_MontageComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_MontageComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_MontageComp
{
	GENERATED_BODY()

public:
	// virtual class UAnimMontage* GetAnimMontage();

	/////////////////////////////////////////////////////////////////////////////////
	// @MontageType - 몽타주 타입(Ex.UMageMontageType, UCommonMontageType)
	// @PlayAnimNum - Type 에 해당하는 애니메이션 순번.
	// @MontageSort - 해당 Actor의 전체 몽타주 종류(MontageSort)(Ex. Move, Sword, Mage...)
	// @Speed - 애니메이션(몽타주) 속도
	// @bAlways - 애니메이션(몽타주) 의 실행성 보장
	// True 면 무조건 실행, False면 애니메이션(몽타주) 실행 시 동작안함.
	virtual void PlayAnimation
	(
		UINT MontageType, UINT PlayMontageNum,
		UINT MontageSort, float Speed = 1.5f, bool bAlways = false
	) {}
	/////////////////////////////////////////////////////////////////////////////////


};
