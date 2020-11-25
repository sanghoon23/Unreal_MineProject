#include "MyPlayerState.h"
#include "Global.h"

void AMyPlayerState::GetPlayerInfo(FPlayerInfo & Out)
{
	Out.MaxHP = PlayerInfo.MaxHP;
	Out.CurrentHP = PlayerInfo.CurrentHP;

	Out.MaxMP = PlayerInfo.MaxMP;
	Out.CurrentMP = PlayerInfo.CurrentMP;

	Out.BarrierAmount = PlayerInfo.BarrierAmount;
	Out.AddSpeed = PlayerInfo.AddSpeed;
	Out.ATK_Coefficient = PlayerInfo.ATK_Coefficient;
	Out.DEF_Coefficient = PlayerInfo.DEF_Coefficient;

	Out.Name = PlayerInfo.Name;
}

void AMyPlayerState::SetPlayerInfo(FPlayerInfo & Info)
{
	PlayerInfo.MaxHP = Info.MaxHP;
	PlayerInfo.CurrentHP = Info.CurrentHP;

	PlayerInfo.MaxMP = Info.MaxMP;
	PlayerInfo.CurrentMP = Info.CurrentMP;

	PlayerInfo.BarrierAmount = Info.BarrierAmount;
	PlayerInfo.AddSpeed = Info.AddSpeed;
	PlayerInfo.ATK_Coefficient = Info.ATK_Coefficient;
	PlayerInfo.DEF_Coefficient = Info.DEF_Coefficient;

	PlayerInfo.Name = Info.Name;
}
