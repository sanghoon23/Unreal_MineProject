#include "CHM_PengMao.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Interface/IC_Player.h"

#include "AI/Controller/CAIC_HM_PengMao.h"
#include "UI/Widget/WG_FloatingCombo.h"

ACHM_PengMao::ACHM_PengMao()
{
	PrimaryActorTick.bCanEverTick = true;

	// Super
	{
		DeathCallFunctionTimer = 10.0f;
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
		HitComp = CreateDefaultSubobject<UCHM_MaoHitComp>(TEXT("HitComp"));
		EquipComp = CreateDefaultSubobject<UCHM_MaoEquipComp>(TEXT("EquipComponent"));
		MeshParticleComponent = CreateDefaultSubobject<UCMeshParticleComp>(TEXT("MeshParticleComp"));
		ATKComp = CreateDefaultSubobject<UCHM_MaoAttackComp>(TEXT("ATKComp"));
	}

	//@LOAD Death Particle
	{
		FString strPath = L"";

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/P_Assa_ForDeathSmoke.P_Assa_ForDeathSmoke'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_DeathSmoke(*strPath);
		if (P_DeathSmoke.Succeeded())
		{
			P_PengMao_DeathSmoke = P_DeathSmoke.Object;
		}
	}

	#pragma region Monster Info Setting

	//# 현재 체력 상태로 갱신해주어야 함.
	Info.MaxHP = 100.0f;
	Info.CurrentHP = 100.0f;
	Info.Name = FName(L"PengMao");
	//Info.InfoConditionDataArray.Init(nullptr, 5);

	#pragma endregion

}

void ACHM_PengMao::BeginPlay()
{
	Super::BeginPlay();

	// @IC_Charactor
	OnActionResetState.AddLambda([&](AActor*)
	{
		CanMove();
		OnGravity();
	});
}

void ACHM_PengMao::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@Death (재가 되어 사라져라..) - OnDeathDelegate 에서 Mesh Change
	{
		if (bDeath == true)
		{
			InsertTimer += DeltaTime;
			if (bInsertForDeathMesh == false && InsertTimer > 3.0f)
			{
				bInsertForDeathMesh = true;

				GetIHitComp()->SettingCustomCharactorMesh(ECharactorMeshSort::FORDEATH);

				//@죽어서 재가 되는 Material, Dynamic 으로 집어넣기
				int32 MatCount = GetMesh()->GetNumMaterials();
				for (int i = 0; i < MatCount; ++i)
				{
					UMaterialInterface* MInst = GetMesh()->GetMaterial(i);
					if (MInst != nullptr)
					{
						UMaterialInstanceDynamic* TargetMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this->GetWorld(), MInst);
						if (TargetMat != nullptr)
						{
							GetMesh()->SetMaterial(i, TargetMat); //@Set
							MatInstDynamicArray.Add(TargetMat);
						}
					}
				}
			}//(bInsertForDeathMesh)

			for (UMaterialInstanceDynamic* Inst : MatInstDynamicArray)
			{
				FMaterialParameterInfo MatInfo;
				MatInfo.Name = "Appearance";
				float OutValue = 0.0f;
				Inst->GetScalarParameterValue(MatInfo, OutValue);

				if (FMath::IsNearlyEqual(OutValue, 0.6f, 0.01f))
				{
					//@죽을 때 연기(Smoke) 나옴.
					FTransform P_Transform;
					P_Transform.SetLocation(GetActorLocation());
					P_Transform.SetScale3D(FVector(4.0f));
					if (P_PengMao_DeathSmoke != nullptr)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_PengMao_DeathSmoke, P_Transform, true);
						P_PengMao_DeathSmoke = nullptr;
					}
				}

				(OutValue > 0.0f)
					? OutValue -= 0.01f * DeltaTime * MatDynamicValueSpeed
					: OutValue -= 0.0f;
				Inst->SetScalarParameterValue(FName("Appearance"), OutValue);
			}//(MatinstDynamicArray)

		}//(bDeath == true)
	}
}

void ACHM_PengMao::GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray)
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
}

void ACHM_PengMao::OnDeath()
{
	bDeath = true;

	//@1. 먼저 바인딩된 Delegate 브로드 캐스트 후,
	OnDeathDelegate.Broadcast(); //@Delegate

////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@2. 현 Charactor 에 관해 최종적으로 호출함.
	//그렇지 않으면 Delegate 는 순서를 따지지 않아서 죽었는데도, AI 가 돌고 있음

	//@띄워졌을 때 사망할 때의 예외,
	OnGravity();

	//@Monster 라면, AI 꺼주기
	SetAIRunningPossible(false);

	//@Collision OFF 가 아니라 "Spectator" 로 설정한다.
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");

	OnDelegateCharactorDestroy();
}

void ACHM_PengMao::OnGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACHM_PengMao::OffGravity()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

bool ACHM_PengMao::IsJumping()
{
	return GetCharacterMovement()->IsFalling();
}

void ACHM_PengMao::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
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
void ACHM_PengMao::ActorStopCurrentAnimMon()
{
	StopAnimMontage(CurrentMontage);
}

void ACHM_PengMao::ActorStopAnimMon(class UAnimMontage* Montage)
{
	check(Montage);
	StopAnimMontage(Montage);
}

void ACHM_PengMao::ActorPausedAnimMonResume()
{
	GetMesh()->GetAnimInstance()->Montage_Resume(CurrentMontage);

	//Test Code
	//bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	//if (bRunningMontage == true)
	//{
	//}
}

void ACHM_PengMao::ActorAnimMonPause()
{
	bool bRunningMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage);
	if (bRunningMontage == true)
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(CurrentMontage);
	}
}

void ACHM_PengMao::OnCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACHM_PengMao::OffCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACHM_PengMao::SetAIAttackMode(bool bValue)
{
	bAIAttackMode = bValue;
}

float ACHM_PengMao::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IfFalseRetResult(CanBeDamaged(), Info.CurrentHP);
	IfTrueRetResult(bDeath == true, Info.CurrentHP);

	//@UI
	{
		IIC_Player* IC_Player = Cast<IIC_Player>(DamageCauser);
		if (IC_Player != nullptr)
		{
			APawn* PlayerTarget = IC_Player->GetFindAttackTarget();
			if (this == PlayerTarget && PlayerTarget != nullptr)
			{
				UWorld* const World = GetWorld();

				FVector InsertPos = GetActorLocation();
				InsertPos.Z += GetDefaultHalfHeight() + 100.0f;

				UWG_FloatingCombo* FloatingComboUI = CreateWidget<UWG_FloatingCombo>(GetWorld(), FloatingComboClass);
				if (FloatingComboUI != nullptr)
				{
					APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@주체자.
					if (PC != nullptr && bUsingFloatingComboUI)
					{
						FloatingComboUI->SetOwner(this);
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
		}
	}

	Info.CurrentHP -= DamageAmount;

	if (Info.CurrentHP <= 0.0f)
	{
		OnDeath();
	}
	return Info.CurrentHP;
}

void ACHM_PengMao::OnDelegateCharactorDestroy()
{
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACHM_PengMao::CallDestory, DeathCallFunctionTimer);
}

void ACHM_PengMao::CallDestory()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

IIC_AttackComp * ACHM_PengMao::GetIAttackComp()
{
	IfTrueRetResult(ATKComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_AttackComp>(ATKComp);
}

IIC_EquipComp * ACHM_PengMao::GetIEquipComp()
{
	IfTrueRetResult(EquipComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_EquipComp>(EquipComp);
}

IIC_HitComp * ACHM_PengMao::GetIHitComp()
{
	IfTrueRetResult(HitComp == nullptr, nullptr); // @Return Null
	return Cast<IIC_HitComp>(HitComp);
}

IIC_MeshParticle * ACHM_PengMao::GetIMeshParticle()
{
	IfTrueRetResult(MeshParticleComponent == nullptr, nullptr); // @Return Null
	return Cast<IIC_MeshParticle>(MeshParticleComponent);
}

AActor * ACHM_PengMao::GetTargetInAI()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	check(AIController);

	AActor* RetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
	check(RetActor);

	return RetActor;
}
