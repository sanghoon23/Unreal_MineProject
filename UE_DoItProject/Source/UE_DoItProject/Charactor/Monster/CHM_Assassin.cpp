#include "CHM_Assassin.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Interface/IC_Player.h"
#include "Component/Base/C_BaseAbilityComp.h"

#include "AI/Controller/CAIC_HM_Assassin.h"
#include "UI/Widget/WG_FloatingCombo.h"

ACHM_Assassin::ACHM_Assassin()
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
		HitComp = CreateDefaultSubobject<UCHM_AssassinHitComp>(TEXT("HitComp"));
		MeshParticleComponent = CreateDefaultSubobject<UCMeshParticleComp>(TEXT("MeshParticleComp"));
		Assassin_ATKComp = CreateDefaultSubobject<UCHM_AssassinAttackComp>(TEXT("Assassin_ATKComp"));
		Assassin_ActionComp = CreateDefaultSubobject<UCHM_AssassinActionComp>(TEXT("Assassin_ActionComp"));
		Assassin_AbilityComp = CreateDefaultSubobject<UC_BaseAbilityComp>(TEXT("Assassin_AbilityComp"));
	}

	//@LOAD Death Particle
	{
		FString strPath = L"";

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/P_Assa_ForDeathSmoke.P_Assa_ForDeathSmoke'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_DeathSmoke(*strPath);
		if (P_DeathSmoke.Succeeded())
		{
			P_Assa_DeathSmoke = P_DeathSmoke.Object;
		}
	}

#pragma region Monster Info Setting

	//# ���� ü�� ���·� �������־�� ��.
	MonsterInfo.MaxHP = 300.0f;
	MonsterInfo.CurrentHP = 200.0f;
	MonsterInfo.Name = FName(L"Assassin");
	//MonsterInfo.InfoConditionDataArray.Init(nullptr, 5);

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
}

void ACHM_Assassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@�г� ����
	if (MonsterInfo.CurrentHP <= MonsterInfo.MaxHP * 0.5f 
		&& AngerState == EAssa_AngerState::NONE
		&& bDeath == false) //HP�� ����
	{
		IIC_ActionComp* I_ActionComp = GetIActionComp();
		check(I_ActionComp);

		IIC_BaseAction* AngerAction = I_ActionComp->GetIBaseAction(static_cast<uint8>(EAssa_ActionType::ANGER));
		check(AngerAction);

		//bool bMonPlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentMontage());
		//IfTrueRet(bMonPlaying);

		/* ���� ��� ȣ��ȴ�..? */
		//CLog::Print(L"OnAction CALL!!");

		ACAIC_HM_Assassin* HM_AssaController = Cast<ACAIC_HM_Assassin>(GetController());
		if (HM_AssaController != nullptr)
		{
			CLog::Print(L"AssaController NOT NULL!!");

			//TODO : �г�Ǹ� ��ų �߰�.
			//�����°� Ȯ������, �ҷ��ͼ� �߰��ϸ��.
			HM_AssaController->AddTypeRandomAttack(EHM_AssassinAttackType::THIRDATTACK);
			HM_AssaController->AddTypeRandomAttack(EHM_AssassinAttackType::FIVEATTACK);
		}

		AngerAction->OnAction();
		AngerState = EAssa_AngerState::DOING;
	}

	//@Death (�簡 �Ǿ� �������..) - OnDeathDelegate ���� Mesh Change
	{
		if (bDeath == true)
		{
			InsertTimer += DeltaTime;
			if (bInsertForDeathMesh == false && InsertTimer > 3.0f)
			{
				bInsertForDeathMesh = true;

				GetIHitComp()->SettingCustomCharactorMesh(ECharactorMeshSort::FORDEATH);

				//@�׾ �簡 �Ǵ� Material, Dynamic ���� ����ֱ�
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
					//@���� �� ����(Smoke) ����.
					FTransform P_Transform;
					P_Transform.SetLocation(GetActorLocation());
					P_Transform.SetScale3D(FVector(4.0f));
					if (P_Assa_DeathSmoke != nullptr)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Assa_DeathSmoke, P_Transform, true);
						P_Assa_DeathSmoke = nullptr;
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

	//@���� ���ε��� Delegate ��ε� ĳ��Ʈ ��,
	OnDeathDelegate.Broadcast(); //@Delegate

////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@�� Charactor �� ���� ���������� ȣ����.
	//�׷��� ������ Delegate �� ������ ������ �ʾƼ� �׾��µ���, AI �� ���� ����

	//@������� �� ����� ���� ����,
	OnGravity();

	//@Monster ���, AI ���ֱ�
	SetAIRunningPossible(false);

	//@Collision OFF �� �ƴ϶� "Spectator" �� �����Ѵ�.
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

	//@Montage �� ���� ����
	IfTrueRet(bDontMontagePlay);

	if (bAlways == false)
	{
		// ���� ĳ���Ͱ� ��Ÿ�� ���� ���̶��,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}

/* ���� ����� CurrentMontage Stop */
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
	MonsterInfo.ATK_Coefficient += fValue;
}

void ACHM_Assassin::AddDEF(float fValue)
{
	Super::AddDEF(fValue);
	MonsterInfo.DEF_Coefficient += fValue;
}

void ACHM_Assassin::SetAIAttackMode(bool bValue)
{
	bAIAttackMode = bValue;
}

float ACHM_Assassin::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IfFalseRetResult(CanBeDamaged(), MonsterInfo.CurrentHP);
	IfTrueRetResult(bDeath == true, MonsterInfo.CurrentHP);

	//TODO : '�����' �� ���⼭ ��� ���ָ��. Ex) DamageAmount/DEF_Coefficient

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
					APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@��ü��.
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

	MonsterInfo.CurrentHP -= DamageAmount;

	if (MonsterInfo.CurrentHP <= 0.0f)
	{
		OnDeath();
	}

	return MonsterInfo.CurrentHP;
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

/* I_Charactor �� CurrentBaseAction Setting */
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

