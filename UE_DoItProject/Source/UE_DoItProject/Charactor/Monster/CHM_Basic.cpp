#include "CHM_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AI/Controller/CAIC_HM_Shaman.h"

ACHM_Basic::ACHM_Basic()
{
	PrimaryActorTick.bCanEverTick = true;

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
		AttackComponent = CreateDefaultSubobject<UCHM_BasicAttackComp>("AttackComp");
		HitComp = CreateDefaultSubobject<UCHM_BasicHitComp>("HitComp");
		EquipComp = CreateDefaultSubobject<UCHM_BasicEquipComp>("EquipComponent");
		MeshParticleComponent = CreateDefaultSubobject<UCMeshParticleComp>("MeshParticleComp");
	}

	#pragma region Monster Info Setting

	//# 현재 체력 상태로 갱신해주어야 함.
	Info.MaxHP = 100.0f;
	Info.CurrentHP = 100.0f;
	Info.Name = FName(L"HM_Basic");
	//Info.InfoConditionDataArray.Init(nullptr, 5);

	#pragma endregion
}

void ACHM_Basic::BeginPlay()
{
	Super::BeginPlay();

	// @IC_Charactor
	OnActionResetState.AddLambda([&](AActor*)
	{
		CanMove();
		OnGravity();
	});

	//#Edit 0528 - 순서가 맞지 않아, OnDeath 함수에서 호출
	//// Set Delegate
	//{
	//	OnDeathDelegate.AddLambda([&]()
	//	{
	//		//OffCollision();
	//		//AutoPossessAI = EAutoPossessAI::Disabled;
	//		//AIControllerClass = nullptr;
	//	});

	//	OnCharactorDestroy.AddUObject(this, &ACHM_Basic::OnDelegateCharactorDestroy);
	//}
}

void ACHM_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACHM_Basic::GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray)
{
	//@HitComp
	TArray<UCBaseConditionType*> ConditionTypes;
	HitComp->GetConditionDatasOutArray(&ConditionTypes);
	for (UCBaseConditionType* ConditionType : ConditionTypes)
	{
		FViewConditionState Insert;
		Insert.TextureUI = ConditionType->GetTextureUI();
		Insert.ColorAndOpacity = ConditionType->ColorAndOpacity;
		Insert.ApplyTime = ConditionType->ApplyTime;

		OutArray->Emplace(Insert);
	}

	// TODO : 
	//@AblilityComp
	//TArray<UCBaseAbility*> Abilities;
}

void ACHM_Basic::OnDeath()
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

void ACHM_Basic::OnGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACHM_Basic::OffGravity()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

bool ACHM_Basic::IsJumping()
{
	return GetCharacterMovement()->IsFalling();
}

void ACHM_Basic::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
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
void ACHM_Basic::ActorStopCurrentAnimMon()
{
	StopAnimMontage(CurrentMontage);
}

void ACHM_Basic::ActorStopAnimMon(class UAnimMontage* Montage)
{
	check(Montage);
	StopAnimMontage(Montage);
}

void ACHM_Basic::ActorPausedAnimMonResume()
{
	GetMesh()->GetAnimInstance()->Montage_Resume(CurrentMontage);

	//Test Code
	//bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	//if (bRunningMontage == true)
	//{
	//}
}

void ACHM_Basic::ActorAnimMonPause()
{
	bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	if (bRunningMontage == true)
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(CurrentMontage);
	}
}

void ACHM_Basic::OnCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACHM_Basic::OffCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACHM_Basic::SetAIAttackMode(bool bValue)
{
	bAIAttackMode = bValue;
}

float ACHM_Basic::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IfFalseRetResult(CanBeDamaged(), Info.CurrentHP);
	IfTrueRetResult(bDeath == true, Info.CurrentHP);

	Info.CurrentHP -= DamageAmount;

	if (Info.CurrentHP <= 0.0f)
	{
		OnDeath();
	}
	return Info.CurrentHP;
}

void ACHM_Basic::OnDelegateCharactorDestroy()
{
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACHM_Basic::CallDestory, 2.0f);
}

void ACHM_Basic::CallDestory()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

IIC_AttackComp * ACHM_Basic::GetIAttackComp()
{
	IfTrueRetResult(AttackComponent == nullptr, nullptr); // @Return Null
	return Cast<IIC_AttackComp>(AttackComponent);
}

IIC_EquipComp * ACHM_Basic::GetIEquipComp()
{
	IfTrueRetResult(EquipComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_EquipComp>(EquipComp);
}

IIC_HitComp * ACHM_Basic::GetIHitComp()
{
	IfTrueRetResult(HitComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_HitComp>(HitComp);
}

IIC_MeshParticle * ACHM_Basic::GetIMeshParticle()
{
	IfTrueRetResult(MeshParticleComponent == nullptr, nullptr); // @Return Null
	return Cast<IIC_MeshParticle>(MeshParticleComponent);
}

AActor * ACHM_Basic::GetTargetInAI()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	check(AIController);

	AActor* RetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
	check(RetActor);

	return RetActor;
}
