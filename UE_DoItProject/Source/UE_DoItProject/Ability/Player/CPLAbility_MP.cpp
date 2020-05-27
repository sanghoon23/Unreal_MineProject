#include "CPLAbility_MP.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_MP::UCPLAbility_MP()
{
	AbilityType = EAbilityType::MP;
}

void UCPLAbility_MP::ApplyAbility()
{
	check(AppliedActor);

	ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
	check(Player);
	Player->AddCurrentMP(AbilityValue.Value);
}
