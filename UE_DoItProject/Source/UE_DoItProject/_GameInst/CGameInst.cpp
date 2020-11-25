#include "CGameInst.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"

void UCGameInst::Init()
{
	//...
	//@Spectator Camera 생성

}

void UCGameInst::OnInitForMapChange()
{
	ExistWorldMonsterList.Empty();
}

void UCGameInst::GetPlayerInfoFromId(FPlayerInfo & Info, int PlayerID)
{
	auto It = PlayerInfoMap.Find(PlayerID);
	if (It != nullptr)
	{
		Info.MaxHP = It->MaxHP;
		Info.CurrentHP = It->CurrentHP;

		Info.MaxMP = It->MaxMP;
		Info.CurrentMP = It->CurrentMP;

		Info.BarrierAmount = It->BarrierAmount;
		Info.AddSpeed = It->AddSpeed;
		Info.ATK_Coefficient = It->ATK_Coefficient;
		Info.DEF_Coefficient = It->DEF_Coefficient;

		Info.Name = It->Name;

		/*#1104_
		우선 따로 넣지 않음
		상태이상 부분은 해제
		*/
	}
	else
	{
		PlayerInfoMap.Add(PlayerID, Info);
	}
}

void UCGameInst::SetPlayerInfoFromId(FPlayerInfo & Info, int PlayerID)
{
	auto It = PlayerInfoMap.Find(PlayerID);
	if (It != nullptr)
	{
		It->MaxHP = Info.MaxHP;
		It->CurrentHP = Info.CurrentHP;

		It->MaxMP = Info.MaxMP;
		It->CurrentMP = Info.CurrentMP;

		It->BarrierAmount = Info.BarrierAmount;
		It->AddSpeed = Info.AddSpeed;
		It->ATK_Coefficient = Info.ATK_Coefficient;
		It->DEF_Coefficient = Info.DEF_Coefficient;

		It->Name = Info.Name;
	}
	else UE_LOG(LogTemp, Warning, L"UCGameInst SetPlayerInfo Not Setting!!");
}

void UCGameInst::AddExistWorldMonster(ACHumanoidMonster * InsertMon)
{
	check(InsertMon);
	ExistWorldMonsterList.Add(InsertMon);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(InsertMon);
	check(I_Charactor);

	I_Charactor->OnDeathDelegate.AddLambda([&]()
	{
		int Index = ExistWorldMonsterList.Find(InsertMon);
		ExistWorldMonsterList.RemoveAt(Index);
	});
}
