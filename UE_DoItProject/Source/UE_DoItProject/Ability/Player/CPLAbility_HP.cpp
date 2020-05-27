#include "CPLAbility_HP.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_HP::UCPLAbility_HP()
{
	AbilityType = EAbilityType::HP;
}

void UCPLAbility_HP::ApplyAbility()
{
	check(AppliedActor);

	ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
	check(Player);
	Player->AddCurrentHP(AbilityValue.Value);
}
