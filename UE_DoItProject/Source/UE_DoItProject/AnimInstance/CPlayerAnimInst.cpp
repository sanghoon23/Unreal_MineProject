#include "CPlayerAnimInst.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"
#include "Component/Player/CPL_EquipComp.h"

UCPlayerAnimInst::UCPlayerAnimInst()
{
}

void UCPlayerAnimInst::NativeBeginPlay()
{
	Player = Cast<ACPlayer>(TryGetPawnOwner());
	IfNullRet(Player);

	Charactor = Cast<IIC_Charactor>(Player);
	IfNullRet(Charactor);
}

void UCPlayerAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(Player);
	// @Move
	Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
	Speed = Player->GetVelocity().Size();

	//// @공중에 떠있으면서, Gravity 가 0.0f 이 아닐 때,
	//if (Player->GetCharacterMovement()->IsFalling()
	//	&& Player->GetCharacterMovement()->GravityScale > 0.0f)
	//{
	//	bInAir = true;
	//}

	// @Evade
	bEvade = Player->GetEvade();

	IfNullRet(Charactor);
	CurrentStateType = Charactor->GetCurrentStateType();
	bArmed = Charactor->GetIEquipComp()->GetArmed();

	//IfNullRet(BaseAttack);
	//bAttackMode = BaseAttack->GetAttackMode();
}