#include "CHMBaseAnimInst.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCHMBaseAnimInst::UCHMBaseAnimInst()
{
}

void UCHMBaseAnimInst::NativeBeginPlay()
{
	Monster = Cast<ACHM_Basic>(TryGetPawnOwner());
	IfNullRet(Monster);

	I_Charactor = Cast<IIC_Charactor>(Monster);
	IfNullRet(I_Charactor);
}

void UCHMBaseAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(Monster);
	Speed = Monster->GetVelocity().Size();
	bInAir = Monster->GetCharacterMovement()->IsFalling();
	Direction = CalculateDirection(Monster->GetVelocity(), Monster->GetActorRotation());
	bIsRunningMontage = Montage_IsPlaying(Monster->GetCurrentApplyedMontage());

	bDeath = I_Charactor->IsDeath();

	//Test Code
	////@��Ÿ�ְ� ���� ���� ��, ������ ȣ��� ��
	//if (bDeath == true && bIsRunningMontage == true)
	//{

	//}
}

