#include "CHM_AssaFiveAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequence.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Charactor/Monster/CHM_Assassin.h"

UCHM_AssaFiveAttack::UCHM_AssaFiveAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<uint8>(EHM_AssaFiveComboType::NONE);
		MaxComboNum = static_cast<uint8>(EHM_AssaFiveComboType::COMBO_SIX);
	}

	FString Path = L"";

#pragma region FirstAttack

	//@LOAD Montage
	{
		UAnimMontage* Assa_FiveAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Attack/HM_Assassin_Anim_FiveAttack_First_Montage.HM_Assassin_Anim_FiveAttack_First_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> Mon_FiveAttack(*Path);
		if (Mon_FiveAttack.Succeeded())
			Assa_FiveAttack = Mon_FiveAttack.Object;

		AttackMontages.Emplace(Assa_FiveAttack);
	}

	//@LOAD Cinema
	{
		Path = L"LevelSequence'/Game/_Mine/_MyBlueprint/Sequencer/Sequencer_Assa_FiveAttackMotion.Sequencer_Assa_FiveAttackMotion'";
		ConstructorHelpers::FObjectFinder<ULevelSequence> Sequence(*Path);
		if (Sequence.Succeeded())
			LevelSequenForAttack = Sequence.Object;
	}

	// TODO : ���� �� �ó׸� ���� �� ��������Ʈ 
	// AI ���ְ�, Player Input ����, -> SetTimer ����, �ó׸��� ���� ��ŭ. 

#pragma endregion

}

void UCHM_AssaFiveAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

	//@IC_Charactor
	I_Charactor = Cast<IIC_Charactor>(HM_Assassin);
	check(I_Charactor);

	//@LOAD Sequencer
	ALevelSequenceActor* OutputSequenceActor;
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequenForAttack,
		PlaybackSettings,
		OutputSequenceActor
	);
}

void UCHM_AssaFiveAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UAnimInstance* OwnerAnimInst = HM_Assassin->GetMesh()->GetAnimInstance();
	if (OwnerAnimInst != nullptr && bAttacking == true) //@������ ������� ��,
	{
		float CurrentMonPos = OwnerAnimInst->Montage_GetPosition(AttackMontages[0]);
		float StartSectionLength, EndSectionLength = 0.0f;
		const int32 NextActionSectionIndex = AttackMontages[0]->GetSectionIndex(FName("NextAction"));
		AttackMontages[0]->GetSectionStartAndEndTime(NextActionSectionIndex, StartSectionLength, EndSectionLength);

		if (CurrentMonPos >= StartSectionLength) //@���� ����(NextAction)
		{
			OwnerAnimInst->Montage_SetPlayRate(AttackMontages[0], NextSectionPlayRate);
		}
	}
}

void UCHM_AssaFiveAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_AssaFiveAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	//@IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(IsLastCombo());

	CLog::Print(L"AssaFiveAttack BeginAttack IN!!");

	if (bAttacking == false)
	{
		//LevelSequencePlayer->Play();

		HM_Assassin->SetAIRunningPossible(false);

		HM_Assassin->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.2f, false
		);

		CLog::Print(L"AssaFiveAttack BeginAttack IN!!");
	}
}

bool UCHM_AssaFiveAttack::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<UINT>(EHM_AssaFiveComboType::COMBO_SIX))
		return true;

	return false;
}

void UCHM_AssaFiveAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	IfTrueRet(HM_Assassin == nullptr);

	//@���� �޺� �÷���
	++CurrentComboNum;
	const uint8 ComboNum = static_cast<uint8>(CurrentComboNum);

	FVector ActorForward = HM_Assassin->GetActorForwardVector();
	FVector Start = HM_Assassin->GetActorLocation();
	FVector End = HM_Assassin->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, HM_Assassin);

	FHitResult HitResult;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult
		, Start
		, End
		, FQuat::Identity
		, I_Charactor->GetCharactorUsingAttackChannel() // @MonsterAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	//DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		IIC_Charactor* HitIneterfaceCharactor = Cast<IIC_Charactor>(HitResult.GetActor());
		check(HitIneterfaceCharactor);

		IIC_HitComp* I_HitComp = HitIneterfaceCharactor->GetIHitComp();
		check(I_HitComp);

		I_HitComp->BeginBeatedFunc.AddUObject(this, &UCHM_AssaFiveAttack::BeginBeatedFunction);

		//@�̵�
		AttackMoveDir = HM_Assassin->GetActorForwardVector();
		AttackMoveSpeed = 1.0f;

		//@�´� ����
		FVector HitDirection = HitResult.GetActor()->GetActorLocation() - HM_Assassin->GetActorLocation();
		HitDirection.Z = 0.0f;
		HitDirection.Normalize();
		I_HitComp->SetHitDirection(HitDirection);

		if (ComboNum == static_cast<uint8>(EHM_AssaFiveComboType::COMBO_ONE))
		{
			//@ĳ���� �տ� ���� & �ٶ󺸱�
			FVector OwnerFwdVec = HM_Assassin->GetActorForwardVector();
			FVector OwnerLocation = HM_Assassin->GetActorLocation();
			HitResult.GetActor()->SetActorLocation(OwnerLocation + (OwnerFwdVec * AttackRange));
			UCFL_ActorAgainst::LookAtTarget(HM_Assassin, HitResult.GetActor());

			//@DamageType
			UCDamageType_Normal* DT_Normal = NewObject<UCDamageType_Normal>();
			DT_Normal->SetDamageImpulse(10.0f);
			I_HitComp->SetHitMoveSpeed(0.2f);
			I_HitComp->OnHit(HM_Assassin, DT_Normal, DT_Normal->DamageImpulse);

			//@Sequencer Play
			LevelSequencePlayer->Play();
		}
		else
		{
			if ((IsLastCombo() == true)) //������ �ϰ�
			{
				//@DamageType
				UCDamageType_StrongAttack* DT_Strong = NewObject<UCDamageType_StrongAttack>();
				DT_Strong->SetDamageImpulse(20.0f);
				I_HitComp->SetHitMoveSpeed(3.0f);
				I_HitComp->OnHit(HM_Assassin, DT_Strong, DT_Strong->DamageImpulse);

				{
					FTimerDelegate EndBeatedTimerDelegate;
					EndBeatedTimerDelegate = FTimerDelegate::CreateUObject(this, &UCHM_AssaFiveAttack::EndBeatedFunction, HitResult.GetActor());

					FTimerHandle EndBeatedTimerHandle;
					HitResult.GetActor()->GetWorldTimerManager().SetTimer
					(
						EndBeatedTimerHandle,
						EndBeatedTimerDelegate,
						SequenceDelayToFinish,
						false
					);
				}

				{
					FTimerDelegate EndAttackMotionDelegate;
					EndAttackMotionDelegate = FTimerDelegate::CreateUObject(this, &UCHM_AssaFiveAttack::EndSequencerMotion, GetOwner());

					FTimerHandle EndAttackMotionHandle;
					HM_Assassin->GetWorldTimerManager().SetTimer
					(
						EndAttackMotionHandle,
						EndAttackMotionDelegate,
						SequenceDelayToFinish,
						false
					);

					//FTimerHandle OpenLevelTimerHandle;
					//FTimerDelegate TimerDelegate;
					//FName LambdaInsertLoadMapName = LoadMapName;
					//TimerDelegate.BindLambda([LoadBackground, World, LambdaInsertLoadMapName]()
					//{
					//	//@Remove
					//	LoadBackground->RemoveFromViewport();

					//	//@Open Level(MapName)
					//	UGameplayStatics::OpenLevel(World, LambdaInsertLoadMapName);
					//});
					//GetWorldTimerManager().SetTimer(OpenLevelTimerHandle, TimerDelegate, 3.0f, false);
				}
			}
			else if (CurrentComboNum == static_cast<uint8>(EHM_AssaFiveComboType::COMBO_FIVE))
			{
				//@DamageType
				UCDamageType_Air* DT_Air= NewObject<UCDamageType_Air>();
				DT_Air->SetDamageImpulse(10.0f);
				I_HitComp->SetHitMoveSpeed(0.1f);
				I_HitComp->OnHit(HM_Assassin, DT_Air, DT_Air->DamageImpulse);
			}
			else
			{
				//@DamageType
				UCDamageType_Normal* DT_Normal = NewObject<UCDamageType_Normal>();
				DT_Normal->SetDamageImpulse(10.0f);
				I_HitComp->SetHitMoveSpeed(0.1f);
				I_HitComp->OnHit(HM_Assassin, DT_Normal, DT_Normal->DamageImpulse);
			}
		}
	}
	else
	{
		HM_Assassin->SetAIRunningPossible(true);
		EndAttack();
	}
}

void UCHM_AssaFiveAttack::EndSequencerMotion(AActor * Subject)
{
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Subject);
	if (I_Monster != nullptr)
	{
		I_Monster->SetAIRunningPossible(true);
	}

	EndAttack();
}

void UCHM_AssaFiveAttack::BeginBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanNotMove();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFiveAttack BeginBetedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OnBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFiveAttack BeginBetedFunc, I_Player NULL!!")
}

void UCHM_AssaFiveAttack::EndBeatedFunction(AActor * Subject)
{
	IIC_Charactor* SubjectCharactorInterface = Cast<IIC_Charactor>(Subject);
	if (SubjectCharactorInterface != nullptr)
	{
		SubjectCharactorInterface->CanMove();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFiveAttack EndBeatedFunc, I_Charactor NULL!!");

	IIC_Player* SubjectPlayerInterface = Cast<IIC_Player>(Subject);
	if (SubjectPlayerInterface != nullptr)
	{
		SubjectPlayerInterface->OffBlockKeyInput();
	}
	else UE_LOG(LogTemp, Warning, L"AssaFiveAttack EndBetedFunc, I_Player NULL!!");
}

