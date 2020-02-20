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

	Charactor = Cast<IIC_Charactor>(Monster);
	IfNullRet(Charactor);
}

void UCHMBaseAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(Monster);
	Speed = Monster->GetVelocity().Size();
	bInAir = Monster->GetCharacterMovement()->IsFalling();
	Direction = CalculateDirection(Monster->GetVelocity(), Monster->GetActorRotation());

	bDeath = Charactor->IsDeath();
}

