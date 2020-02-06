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
	// @MontageType - ��Ÿ�� Ÿ��(Ex.UMageMontageType, UCommonMontageType)
	// @PlayAnimNum - Type �� �ش��ϴ� �ִϸ��̼� ����.
	// @MontageSort - �ش� Actor�� ��ü ��Ÿ�� ����(MontageSort)(Ex. Move, Sword, Mage...)
	// @Speed - �ִϸ��̼�(��Ÿ��) �ӵ�
	// @bAlways - �ִϸ��̼�(��Ÿ��) �� ���༺ ����
	// True �� ������ ����, False�� �ִϸ��̼�(��Ÿ��) ���� �� ���۾���.
	virtual void PlayAnimation
	(
		UINT MontageType, UINT PlayMontageNum,
		UINT MontageSort, float Speed = 1.5f, bool bAlways = false
	) {}
	/////////////////////////////////////////////////////////////////////////////////


};
