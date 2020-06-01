#include "CS_TargetingSystem.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Figure/CPlaneActor.h"

//UI
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_TargetInfo.h"

UCS_TargetingSystem::UCS_TargetingSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	FString strPath = L"";

	strPath = L"Material'/Game/_Mine/UseMaterial/Mat_TargetMark.Mat_TargetMark'";
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat_TargetMark(*strPath);
	if (Mat_TargetMark.Succeeded())
	{
		Mat_TargetMarkActor = Mat_TargetMark.Object;
	}
}


// Called when the game starts
void UCS_TargetingSystem::BeginPlay()
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

	//@Spawn PlaneActor & Set Material
	FTransform Transform = FTransform::Identity;
	TargetMarkActor = GetWorld()->SpawnActor<ACPlaneActor>(ACPlaneActor::StaticClass(), Transform);
	TargetMarkActor->SetActorScale3D(FVector(2.0f));
	TargetMarkActor->SetMaterial(Mat_TargetMarkActor);
	TargetMarkActor->GetRootComponent()->SetVisibility(false);
}


// Called every frame
void UCS_TargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//@Target 이 죽었을 때,
	IIC_Charactor* I_TargetCharactor = Cast<IIC_Charactor>(CurrentFindAttackTarget);
	if (I_TargetCharactor != nullptr)
	{
		if (I_TargetCharactor->IsDeath())
		{
			//@Target Selected 에서 제외
			DelSelectedMonstersArray(CurrentFindAttackTarget);

			//@Target NULL 시키기.
			CurrentFindAttackTarget = nullptr;

			//@Widget UnVisible
			TargetInfoWidget->WigetUnVisible();

			//@MarkActor Visible OFF
			TargetMarkActor->SetSMVisibility(false);

			return;
		}
		else
		{
			//@MarkActor Set Location
			FVector TargetLocation = CurrentFindAttackTarget->GetActorLocation();
			TargetMarkActor->SetActorLocation(TargetLocation);

			//@MarkActor Visible ON
			TargetMarkActor->SetSMVisibility(true);
		}
	}

	//@ESC 를 눌렀을 때,
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		if (PlayerController->IsInputKeyDown(EKeys::Escape))
		{
			//@Target Selected 에서 제외
			//DelSelectedMonstersArray(CurrentFindAttackTarget);
			SelectedMonsters.Empty();

			//@Target NULL 시키기
			CurrentFindAttackTarget = nullptr;

			//@Widget UnVisible
			TargetInfoWidget->WigetUnVisible();

			//@MarkActor Visible OFF
			TargetMarkActor->SetSMVisibility(false);
		}
	}
}

/* TargetingSystem TarceChannel 을 이용해 Target 선별하기. */
void UCS_TargetingSystem::OnFindTargets(FVector CenterPos, float CollisionSphereRadius)
{
	FVector Center = CenterPos;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(CollisionSphereRadius);
	FCollisionQueryParams Params(NAME_None, false, GetOwner());

	TArray<FOverlapResult> overlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel
	(
		overlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //@TragetSystem TraceChannel
		Sphere,
		Params
	);

	float DebugLiftTime = 2.0f;
#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), Center, Sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLiftTime);

#endif //  ENABLE_DRAW_DEBUG

	overlapResults.StableSort([&](const FOverlapResult& A, const FOverlapResult& B)
	{
		float DistanceToA = Player->GetDistanceTo(A.GetActor());
		float DistanceToB = Player->GetDistanceTo(B.GetActor());

		if (DistanceToA < DistanceToB)
			return true;
		else return false;
	});

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

				//@더 이상 선택되지않은 애들이 들어왔을 때, 초기화
				if (i == 0)
				{
					SelectedMonsters.Empty();
				}

				//@Target
				CurrentFindAttackTarget = FindMonster;

				//@Array ADD - (Sort)
				AddSelectedMonstersArray(FindMonster);

				//@Widget Visible
				TargetInfoWidget->WigetVisible();
				return;
			}
		}
	}
}

bool UCS_TargetingSystem::CheckSelectedMonsters(APawn * InputPawn)
{
	check(InputPawn);
	for (APawn* Pawn : SelectedMonsters)
	{
		if (Pawn == InputPawn)
			return true;
	}

	return false;
}

void UCS_TargetingSystem::AddSelectedMonstersArray(APawn * InputPawn)
{
	check(InputPawn);
	SelectedMonsters.Add(InputPawn);
}

void UCS_TargetingSystem::DelSelectedMonstersArray(APawn * InputPawn)
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

APawn * UCS_TargetingSystem::GetSelectedMonsterArray(uint8 Index)
{
	if (Index >= SelectedMonsters.Num() || Index < 0)
		return nullptr;

	return SelectedMonsters[Index];
}

APawn * UCS_TargetingSystem::GetCurrentFindAttackTarget()
{
	//@죽었을 때,
	IIC_Charactor* Charactor = Cast<IIC_Charactor>(CurrentFindAttackTarget);
	if (Charactor != nullptr)
	{
		bool bDeath = Charactor->IsDeath();
	}
	if (CurrentFindAttackTarget == nullptr)
		return nullptr;

	return CurrentFindAttackTarget;
}
