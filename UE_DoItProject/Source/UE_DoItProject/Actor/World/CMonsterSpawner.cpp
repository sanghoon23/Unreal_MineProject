#include "CMonsterSpawner.h"
#include "Global.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "_GameInst/CGameInst.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequence.h"

#include "Interface/IC_Player.h"
#include "Interface/IC_Monster.h"

ACMonsterSpawner::ACMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	//@LOAD MonsterSpawnSeq
	{
		if (SeqMonsterSpawn != nullptr)
		{
			ALevelSequenceActor* OutputSequenceActor;
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			MonsterSpawnSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
			(
				GetWorld(),
				SeqMonsterSpawn,
				PlaybackSettings,
				OutputSequenceActor
			);
			check(MonsterSpawnSequencePlayer);
			MonsterSpawnSequencePlayer->OnPlay.AddDynamic(this, &ACMonsterSpawner::SpawnSequencePlay);
			MonsterSpawnSequencePlayer->OnFinished.AddDynamic(this, &ACMonsterSpawner::SpawnSequenceFinished);
		}
	}

	//@LOAD NextStageSeq
	{
		if (SeqNextStage != nullptr)
		{
			ALevelSequenceActor* OutputSequenceActor;
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			NextStageSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
			(
				GetWorld(),
				SeqNextStage,
				PlaybackSettings,
				OutputSequenceActor
			);
			check(NextStageSequencePlayer);
			NextStageSequencePlayer->OnPlay.AddDynamic(this, &ACMonsterSpawner::DeadSequencePlay);
			NextStageSequencePlayer->OnFinished.AddDynamic(this, &ACMonsterSpawner::DeadSequenceFinished);
		}
	}
}

void ACMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IfTrueRet(bExit);

	if (bHitting == false)
	{
		CheckHittingInDetectRadius();
	}
	else
	{
		if (GetWorld()->GetGameInstance<UCGameInst>()->IsExistWorldMonster() == false)
		{
			if (MonsterSpawnSequencePlayer == nullptr || 
				(MonsterSpawnSequencePlayer != nullptr && MonsterSpawnSequencePlayer->IsPlaying() == false))
			{
				if (NextStageSequencePlayer != nullptr)
				{
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACMonsterSpawner::TimerFunc, FuncTimerOfNotExistMon);
					bExit = true;
				}
			}
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void ACMonsterSpawner::CheckHittingInDetectRadius()
{
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + GetActorForwardVector() * DetectRadius;

	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, this);

	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult
		, Start
		, End
		, FQuat::Identity
		, ECollisionChannel::ECC_GameTraceChannel3 // @MonsterAttack
		, Sphere
		, CollisionQueryParm

	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), End, Sphere.GetSphereRadius(), 40, FColor::Green, false, 1.0f);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		IIC_Player* IC_Player = Cast<IIC_Player>(HitResult.GetActor());
		if (IC_Player != nullptr)
		{
			//@Register GameInst
			for (ACHumanoidMonster* Monster : SeqMonsterSpawnList)
			{
				IIC_Monster* I_Monster = Cast<IIC_Monster>(Monster);
				check(I_Monster);
				I_Monster->SetAIRunningPossible(true);

				//@등록
				GetWorld()->GetGameInstance<UCGameInst>()->AddExistWorldMonster(Monster);
			}

			HittingPlayer = HitResult.GetActor();
			if (MonsterSpawnSequencePlayer != nullptr)
			{
				MonsterSpawnSequencePlayer->Play();
			}
			else UE_LOG(LogTemp, Warning, L"CMonsterSpawner, SequencePlayer NULL!!");

			bHitting = true;
		}
	}
}

void ACMonsterSpawner::TimerFunc()
{
	NextStageSequencePlayer->Play();
}

void ACMonsterSpawner::SpawnSequencePlay()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PC);

	ACharacter* OwnerCH = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(OwnerCH);
	OwnerCH->DisableInput(PC);
}

void ACMonsterSpawner::SpawnSequenceFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PC);

	ACharacter* OwnerCH = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(OwnerCH);
	OwnerCH->EnableInput(PC);
}

void ACMonsterSpawner::DeadSequencePlay()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PC);

	ACharacter* OwnerCH = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(OwnerCH);
	OwnerCH->DisableInput(PC);
}

void ACMonsterSpawner::DeadSequenceFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PC);

	ACharacter* OwnerCH = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(OwnerCH);
	OwnerCH->EnableInput(PC);

	//@삭제
	Destroy();
}
