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

	I_Monster = Cast<IIC_Monster>(Monster);
	IfNullRet(I_Monster);
}

void UCHMBaseAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(Monster);
	Speed = Monster->GetVelocity().Size();
	bInAir = Monster->GetCharacterMovement()->IsFalling();
	Direction = CalculateDirection(Monster->GetVelocity(), Monster->GetActorRotation());
	bIsRunningMontage = Montage_IsPlaying(Monster->GetCurrentApplyedMontage());

	bDeath = I_Charactor->IsDeath();

	bAttackMode = I_Monster->GetAIAttackMode();
}

