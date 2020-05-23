#include "CPL_TargetingSystem.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Charactor/Player/CPlayer.h"

//UI
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_TargetInfo.h"

UCPL_TargetingSystem::UCPL_TargetingSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_TargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	//@Set Player
	Player = Cast<ACPlayer>(GetOwner());

	//@UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		TargetInfoWidget = MainHUD->GetWidgetTargetInfo();
	}
}


void UCPL_TargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//@���Ͱ� �׾��� ��,
	IIC_Charactor* I_TargetCharactor = Cast<IIC_Charactor>(CurrentFindAttackTarget);
	if (I_TargetCharactor != nullptr)
	{
		if (I_TargetCharactor->IsDeath())
		{
			//@Target Selected ���� ����
			DelSelectedMonstersArray(CurrentFindAttackTarget);

			//@Target NULL ��Ű��.
			CurrentFindAttackTarget = nullptr;

			//@Widget UnVisible
			TargetInfoWidget->WigetUnVisible();

			return;
		}
	}

	//@ESC �� ������ ��,
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		if (PlayerController->IsInputKeyDown(EKeys::Escape))
		{
			//@Target Selected ���� ����
			DelSelectedMonstersArray(CurrentFindAttackTarget);

			//@Target NULL ��Ű��
			CurrentFindAttackTarget = nullptr;

			//@Widget UnVisible
			TargetInfoWidget->WigetUnVisible();
		}
	}
}

/* TargetingSystem TarceChannel �� �̿��� Target �����ϱ�. */
void UCPL_TargetingSystem::OnFindTargets()
{
	FVector Center = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(FindDistance);
	FCollisionQueryParams params(NAME_None, false, GetOwner());

	TArray<FOverlapResult> overlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel
	(
		overlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //@TragetSystem TraceChannel
		Sphere,
		params
	);

	float DebugLiftTime = 2.0f;
#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), Center, Sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLiftTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		for (int i = 0; i < overlapResults.Num(); ++i)
		{
			IIC_Monster* I_Monster = Cast<IIC_Monster>(overlapResults[i].GetActor());
			if (I_Monster != nullptr)
			{
				APawn* FindMonster = Cast<APawn>(I_Monster);
				check(FindMonster);
				int LastIndex = overlapResults.Num() - 1;
				if (i != LastIndex && CheckSelectedMonsters(FindMonster) == true)
				{
					continue;
				}
				else if (i == LastIndex && CheckSelectedMonsters(FindMonster) == true)
				{
					//@Change Pointer FindMonster
					FindMonster = GetSelectedMonsterArray(0);
					SelectedMonsters.Empty();
				}

				//@�� �̻� ���õ������� �ֵ��� ������ ��, �ʱ�ȭ
				if (i == 0)
				{
					SelectedMonsters.Empty();
				}

				//@Target
				CurrentFindAttackTarget = FindMonster;

				//@Array ADD
				AddSelectedMonstersArray(FindMonster);

				//@Widget Visible
				TargetInfoWidget->WigetVisible();
				return; 
			}
		}
	}
}

bool UCPL_TargetingSystem::CheckSelectedMonsters(APawn * InputPawn)
{
	check(InputPawn);
	for (APawn* Pawn : SelectedMonsters)
	{
		if (Pawn == InputPawn)
			return true;
	}

	return false;
}

void UCPL_TargetingSystem::AddSelectedMonstersArray(APawn * InputPawn)
{
	check(InputPawn);
	SelectedMonsters.Add(InputPawn);
}

void UCPL_TargetingSystem::DelSelectedMonstersArray(APawn * InputPawn)
{
	check(InputPawn);
	for (int i = 0; i < SelectedMonsters.Num(); ++i)
	{
		if (SelectedMonsters[i] == InputPawn)
		{
			SelectedMonsters.RemoveAt(i);
			break;
		}
	}
}

APawn * UCPL_TargetingSystem::GetSelectedMonsterArray(uint8 Index)
{
	if (Index >= SelectedMonsters.Num() || Index < 0)
		return nullptr;

	return SelectedMonsters[Index];
}

APawn * UCPL_TargetingSystem::GetCurrentFindAttackTarget()
{
	//@�׾��� ��,
	IIC_Charactor* Charactor = Cast<IIC_Charactor>(CurrentFindAttackTarget);
	if (Charactor != nullptr)
	{
		bool bDeath = Charactor->IsDeath();
	}
	if (CurrentFindAttackTarget == nullptr)
		return nullptr;

	return CurrentFindAttackTarget;
}

