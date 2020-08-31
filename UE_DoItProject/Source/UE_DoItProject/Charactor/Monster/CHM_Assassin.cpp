#include "CHM_Assassin.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/Base/C_BaseAbilityComp.h"

#include "AI/Controller/CAIC_HM_PengMao.h"
#include "UI/Widget/WG_FloatingCombo.h"

ACHM_Assassin::ACHM_Assassin()
{
	PrimaryActorTick.bCanEverTick = true;

	// Super
	{
		DeathCallFunctionTimer = 5.0f;
	}

	// Default Setting
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	}

	// Create Component
	{
		HitComp = CreateDefaultSubobject<UCHM_AssassinHitComp>(TEXT("HitComp"));
		MeshParticleComponent = CreateDefaultSubobject<UCMeshParticleComp>(TEXT("MeshParticleComp"));
		Assassin_ATKComp = CreateDefaultSubobject<UCHM_AssassinAttackComp>(TEXT("Assassin_ATKComp"));
		Assassin_ActionComp = CreateDefaultSubobject<UCHM_AssassinActionComp>(TEXT("Assassin_ActionComp"));
		Assassin_AbilityComp = CreateDefaultSubobject<UC_BaseAbilityComp>(TEXT("Assassin_AbilityComp"));

		//AddOwnedComponent(AttackComponent);
		//AddOwnedComponent(HitComp);
		//AddOwnedComponent(AttackComponent);
		//AddOwnedComponent(AttackComponent);
	}

	FString strPath = L"";

	strPath = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Assassin/SM_Countess_ForDeath.SM_Countess_ForDeath'";
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK(*strPath);
	if (SK.Succeeded())
	{
		Assa_DeathMesh = SK.Object;
		//SkeletalMesh->SetSkeletalMesh(SK.Object);
	}

	strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/P_Assa_ForDeathSmoke.P_Assa_ForDeathSmoke'";
	ConstructorHelpers::FObjectFinder<UParticleSystem> P_DeathSmoke(*strPath);
	if (P_DeathSmoke.Succeeded())
	{
		P_Assa_DeathSmoke = P_DeathSmoke.Object;
	}

#pragma region Monster Info Setting

	//# 현재 체력 상태로 갱신해주어야 함.
	Info.MaxHP = 300.0f;
	Info.CurrentHP = 10.0f;
	Info.Name = FName(L"Assassin");
	//Info.InfoConditionDataArray.Init(nullptr, 5);

#pragma endregion

}

void ACHM_Assassin::BeginPlay()
{
	Super::BeginPlay();

	// @IC_Charactor
	OnActionResetState.AddLambda([&](AActor*)
	{
		CanMove();
		OnGravity();
	});

	// TODO :
	OnDeathDelegate.AddLambda([&]()
	{
		GetMesh()->SetSkeletalMesh(Assa_DeathMesh);

		//@죽을 때 연기(Smoke) 나옴.
		FTransform P_Transform;
		P_Transform.SetLocation(GetActorLocation());
		P_Transform.SetScale3D(FVector(4.0f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Assa_DeathSmoke, P_Transform, true);
	});

}

void ACHM_Assassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@분노 영역
	if (Info.CurrentHP <= Info.MaxHP * 0.5f && AngerState == EAssa_AngerState::NONE)//HP반 이하
	{
		IIC_ActionComp* I_ActionComp = GetIActionComp();
		check(I_ActionComp);

		IIC_BaseAction* AngerAction = I_ActionComp->GetIBaseAction(static_cast<uint8>(EAssa_ActionType::ANGER));
		check(AngerAction);

		bool bMonPlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentMontage());
		IfTrueRet(bMonPlaying);

		/* 지금 계속 호출된다..? */
		//CLog::Print(L"OnAction CALL!!");

		AngerAction->OnAction();
		AngerState = EAssa_AngerState::DOING;
	}

	//Test Code
	////@Death (재가 되어 사라져라..) - OnDeathDelegate 에서 Mesh Change
	//{
	//	if (bDeath == true)
	//	{
	//		TArray<UMaterialInterface*> Temp;
	//		GetMesh()->GetUsedMaterials(Temp);
	//		for (UMaterialInterface* MInst : Temp)
	//		{
	//			//MInst.setvector
	//		}
	//	}
	//}
}

void ACHM_Assassin::GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray)
{
	//@HitComp
	TArray<UCBaseConditionType*> ConditionTypes;
	HitComp->GetConditionDatasOutArray(&ConditionTypes);
	for (UCBaseConditionType* ConditionType : ConditionTypes)
	{
		FViewConditionState Insert;
		Insert.TextureUI = ConditionType->GetTextureUI();
		Insert.ColorAndOpacity = ConditionType->ColorAndOpacity;
		Insert.TintSlateColor = FSlateColor(ConditionType->TintColor);
		Insert.ApplyTime = ConditionType->ApplyTime;

		OutArray->Emplace(Insert);
	}

	//@AbilityComp
	TArray<UCBaseAbility*> Abilities;
	Assassin_AbilityComp->GetAbilities(Abilities);
	for (UCBaseAbility* Ability : Abilities)
	{
		FViewConditionState Insert;
		Insert.TextureUI = Ability->GetTextureUI();
		Insert.ColorAndOpacity = Ability->ColorAndOpacity;
		Insert.TintSlateColor = FSlateColor(Ability->TintColor);
		Insert.ApplyTime = Ability->GetApplyTimer();

		OutArray->Emplace(Insert);
	}
}

void ACHM_Assassin::OnDeath()
{
	bDeath = true;

	//@1. 먼저 바인딩된 Delegate 브로드 캐스트 후,
	OnDeathDelegate.Broadcast(); //@Delegate

////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@2. 현 Charactor 에 관해 최종적으로 호출함.
	//그렇지 않으면 Delegate 는 순서를 따지지 않아서 죽었는데도, AI 가 돌고 있음

	SetActorTickEnabled(false);

	//@띄워졌을 때 사망할 때의 예외,
	OnGravity();

	//@Monster 라면, AI 꺼주기
	SetAIRunningPossible(false);

	//@Collision OFF 가 아니라 "Spectator" 로 설정한다.
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");

	OnDelegateCharactorDestroy();
}

void ACHM_Assassin::OnGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACHM_Assassin::OffGravity()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

bool ACHM_Assassin::IsJumping()
{
	return GetCharacterMovement()->IsFalling();
}

void ACHM_Assassin::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
{
	check(Montage);

	//@Montage 를 받지 않음
	IfTrueRet(bDontMontagePlay);

	if (bAlways == false)
	{
		// 만약 캐릭터가 몽타주 실행 중이라면,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}

/* 현재 적용된 CurrentMontage Stop */
void ACHM_Assassin::ActorStopCurrentAnimMon()
{
	StopAnimMontage(CurrentMontage);
}

void ACHM_Assassin::ActorStopAnimMon(class UAnimMontage* Montage)
{
	check(Montage);
	StopAnimMontage(Montage);
}

void ACHM_Assassin::ActorPausedAnimMonResume()
{
	GetMesh()->GetAnimInstance()->Montage_Resume(CurrentMontage);

	//Test Code
	//bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	//if (bRunningMontage == true)
	//{
	//}
}

void ACHM_Assassin::ActorAnimMonPause()
{
	bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	if (bRunningMontage == true)
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(CurrentMontage);
	}
}

void ACHM_Assassin::OnCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACHM_Assassin::OffCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACHM_Assassin::AddATK(float fValue)
{
	Super::AddATK(fValue);
	Info.ATK_Coefficient += fValue;
}

void ACHM_Assassin::AddDEF(float fValue)
{
	Super::AddDEF(fValue);
	Info.DEF_Coefficient += fValue;
}

void ACHM_Assassin::SetAIAttackMode(bool bValue)
{
	bAIAttackMode = bValue;
}

float ACHM_Assassin::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IfFalseRetResult(CanBeDamaged(), Info.CurrentHP);
	IfTrueRetResult(bDeath == true, Info.CurrentHP);

	//TODO : '방어계수' 는 여기서 계산 해주면됨. Ex) DamageAmount/DEF_Coefficient

	//@UI
	{
		UWorld* const World = GetWorld();

		FVector InsertPos = GetActorLocation();

		UWG_FloatingCombo* FloatingComboUI = CreateWidget<UWG_FloatingCombo>(GetWorld(), FloatingComboClass);
		if (FloatingComboUI != nullptr)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@주체자.
			if (PC != nullptr && bUsingFloatingComboUI)
			{
				FloatingComboUI->SetInitial(PC, InsertPos, EFloatingComboColor::WHITE);
				FloatingComboUI->SetDisplayDamageValue(DamageAmount);

				FloatingComboUI->AddToViewport();
			}
			else
			{
				bUsingFloatingComboUI = true;
			}
		}
	}

	Info.CurrentHP -= DamageAmount;

	if (Info.CurrentHP <= 0.0f)
	{
		OnDeath();
	}

	return Info.CurrentHP;
}

void ACHM_Assassin::OnDelegateCharactorDestroy()
{
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACHM_Assassin::CallDestory, DeathCallFunctionTimer);
}

void ACHM_Assassin::CallDestory()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

/* I_Charactor 에 CurrentBaseAction Setting */
void ACHM_Assassin::SetCurrentBaseAction(IIC_BaseAction * IBaseAction)
{
	check(IBaseAction);
	CurrentIBaseAction = IBaseAction;
}

IIC_AttackComp * ACHM_Assassin::GetIAttackComp()
{
	IfTrueRetResult(Assassin_ATKComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_AttackComp>(Assassin_ATKComp);
}

IIC_ActionComp * ACHM_Assassin::GetIActionComp()
{
	IfTrueRetResult(Assassin_ActionComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_ActionComp>(Assassin_ActionComp);;
}

IIC_HitComp * ACHM_Assassin::GetIHitComp()
{
	IfTrueRetResult(HitComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_HitComp>(HitComp);
}

IIC_MeshParticle * ACHM_Assassin::GetIMeshParticle()
{
	IfTrueRetResult(MeshParticleComponent == nullptr, nullptr); // @Return Null
	return Cast<IIC_MeshParticle>(MeshParticleComponent);
}

IIC_AbilityComp * ACHM_Assassin::GetIAbilityComp()
{
	return Cast<IIC_AbilityComp>(Assassin_AbilityComp);
}

AActor * ACHM_Assassin::GetTargetInAI()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	check(AIController);

	AActor* RetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
	check(RetActor);

	return RetActor;
}

