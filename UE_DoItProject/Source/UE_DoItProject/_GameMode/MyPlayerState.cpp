#include "MyPlayerState.h"
#include "Global.h"

void AMyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//PlayerInfo.MaxHP = 1000.0f;
	//PlayerInfo.CurrentHP = 1000.0f;

	//PlayerInfo.MaxMP = 100.0f;
	//PlayerInfo.CurrentMP = 50.0f;

	//PlayerInfo.Name = FName(L"PlayerName");

	//{
	//	1000.0f,	//@MaxHP
	//		1000.0f,	//@CurrentHP
	//		100.0f,		//@MaxMP
	//		50.0f,		//@CurrentMP
	//		0.0f,		//@BarrierAmount
	//		0.0f,		//@AddSpeed
	//		1.0f,		//@ATK_Coefficient
	//		1.0f,		//@DEF_Coefficient
	//};
}

void AMyPlayerState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);

	//CLog::Print(L"PlayerState - CopyProperties Call!!");
	if (IsValid(PlayerState))
	{
		AMyPlayerState* MyPS = Cast<AMyPlayerState>(PlayerState);
		if (MyPS != nullptr)
		{
			CLog::Print(MyPS->PlayerInfo.CurrentHP);
			this->GetPlayerInfo(MyPS->PlayerInfo);
		}
	}
}

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
