#include "WG_PlayerDeadMenu.h"
#include "CLog.h"
#include "UMG.h"
#include "TimerManager.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequence.h"

#include "Charactor/Player/CPlayer.h"

UWG_PlayerDeadMenu::UWG_PlayerDeadMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//@LOAD Cinema
	FString Path = L"";
	{
		Path = L"LevelSequence'/Game/_Mine/_MyBlueprint/Sequencer/FadeSequencer_ForDeadPlayer.FadeSequencer_ForDeadPlayer'";
		ConstructorHelpers::FObjectFinder<ULevelSequence> Sequence(*Path);
		if (Sequence.Succeeded())
			FadeOutDisplay = Sequence.Object;
	}
}

void UWG_PlayerDeadMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Player = GetOwningPlayerPawn<ACPlayer>();
	check(Player);
}

void UWG_PlayerDeadMenu::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (LevelSequencePlayer != nullptr)
	//{
	//	FQualifiedFrameTime QFT = LevelSequencePlayer->GetCurrentTime();
	//	CLog::Print((float)QFT.AsSeconds());
	//}
}

void UWG_PlayerDeadMenu::StartingDeadMenu()
{
	UWorld* const World = GetWorld();
	check(World);

	//@LOAD Sequencer
	ALevelSequenceActor* OutputSequenceActor;
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bPauseAtEnd = true; //@마지막 멈춤.
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
	(
		World,
		FadeOutDisplay,
		PlaybackSettings,
		OutputSequenceActor
	);
	check(LevelSequencePlayer);
	LevelSequencePlayer->Play();

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &UWG_PlayerDeadMenu::WigetVisible, 4.0f);
}

void UWG_PlayerDeadMenu::WigetVisible()
{
	/*
	1118_
	UWG_PlayerDeadMenu 는 SelfHitTestInvisible,
	버튼이 Visible, 로 설정.
	그냥 HitTestInvisble 시, 버튼까지 상속되어버려서 OnClicked 실행 안됨.
	*/
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	/* 1123_ SelfHitTestInvisible 로 설정하면 BP 에서 Animation 실행.. */
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	check(WidgetClass);

	for (int i = 0; i < WidgetClass->Animations.Num(); ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();
		//CLog::Print(Name);
		PlayAnimation(WidgetClass->Animations[i]);
	}
}

void UWG_PlayerDeadMenu::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (LevelSequencePlayer != nullptr)
	{
		LevelSequencePlayer->Stop();
	}
}

void UWG_PlayerDeadMenu::RespawnFunc()
{
	check(Player);
	Player->OnRespawn();

	WigetUnVisible();
}

void UWG_PlayerDeadMenu::SpeactatorModeFunc()
{
	check(Player);
	Player->OnUseSpeatatorMode();

	WigetUnVisible();
}
