#include "CPlayerAnimInst.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
	// Direction = CalculateDirection(Player->GetVelocity(), Player->GetMesh()->);

	//IfNullRet(Charactor);
	//CurrentWeaponNum = Charactor->GetEquipmentComp()->GetCurrentWeaponNum();
	//bArmed = Charactor->GetEquipmentComp()->GetArmed();

	//IfNullRet(BaseAttack);
	//bAttackMode = BaseAttack->GetAttackMode();
}