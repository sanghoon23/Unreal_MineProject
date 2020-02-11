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
	Speed = Player->GetVelocity().Size();
	bInAir = Player->GetCharacterMovement()->IsFalling();
	bEvade = Player->GetEvade();
	Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());

	IfNullRet(Charactor);
	CurrentStateType = Charactor->GetCurrentStateType();
	bArmed = Charactor->GetIEquipComp()->GetArmed();

	//IfNullRet(BaseAttack);
	//bAttackMode = BaseAttack->GetAttackMode();
}