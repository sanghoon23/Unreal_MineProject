#include "IC_BaseAttack.h"
#include "Global.h"

void IIC_BaseAttack::SetOwnerPawn(APawn * Owner)
{
	IfNullRet(Owner);
	OwnerPawn = Owner;
}
