#include "CMonsterSpawner.h"
#include "Global.h"

#include "_GameInst/CGameInst.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequence.h"

#include "Interface/IC_Player.h"
#include "Interface/IC_Monster.h"

ACMonsterSpawner::ACMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	//@LOAD Cinema
	FString Path = L"";
	{
		Path = L"LevelSequence'/Game/_Mine/_MyBlueprint/Sequencer/Stage_1_MonsterSpawnSeq.Stage_1_MonsterSpawnSeq'";
		ConstructorHelpers::FObjectFinder<ULevelSequence> Sequence(*Path);
		if (Sequence.Succeeded())
			SeqMonsterSpawn = Sequence.Object;
	}
}

void ACMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	//@LOAD Sequencer
	ALevelSequenceActor* OutputSequenceActor;
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
	(
		GetWorld(),
		SeqMonsterSpawn,
		PlaybackSettings,
		OutputSequenceActor
	);
	check(LevelSequencePlayer);
}

void ACMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IfTrueRet(bHitting); //@충돌되었다면 Return
////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		if(IC_Player != nullptr)
		{
			//@KeyBlock
			IC_Player->OnBlockKeyInput();

			HittingPlayer = HitResult.GetActor();
			LevelSequencePlayer->OnFinished.AddDynamic(this, &ACMonsterSpawner::SequenceFinished);
			LevelSequencePlayer->Play();

			bHitting = true;
		}
	}

}

void ACMonsterSpawner::SequenceFinished()
{
	check(HittingPlayer);

	UWorld* const World = HittingPlayer->GetWorld();
	check(World);

	IIC_Player* I_Player = Cast<IIC_Player>(HittingPlayer);
	if (I_Player != nullptr)
	{
		I_Player->OffBlockKeyInput();
	}

	for (ACHumanoidMonster* Monster : SeqMonsterSpawnList)
	{
		IIC_Monster* I_Monster = Cast<IIC_Monster>(Monster);
		check(I_Monster);
		I_Monster->SetAIRunningPossible(true);

		//@등록
		GetWorld()->GetGameInstance<UCGameInst>()->AddExistWorldMonster(Monster);
	}

	Destroy(); //@삭제
}
