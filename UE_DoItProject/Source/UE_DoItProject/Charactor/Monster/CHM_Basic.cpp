#include "CHM_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Interface/IC_Player.h"
#include "Interface/IC_HitComp.h"

#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "AI/Controller/CAIC_HM_Shaman.h"
#include "UI/Widget/WG_FloatingCombo.h"

ACHM_Basic::ACHM_Basic()
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
		AttackComponent = CreateDefaultSubobject<UCHM_BasicAttackComp>("AttackComp");
		HitComp = CreateDefaultSubobject<UCHM_BasicHitComp>("HitComp");
		EquipComp = CreateDefaultSubobject<UCHM_BasicEquipComp>("EquipComponent");
		MeshParticleComponent = CreateDefaultSubobject<UCMeshParticleComp>("MeshParticleComp");
	}

	//@LOAD Death Particle
	{
		FString strPath = L"";

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Basic/P_Basic_ForDeathSmoke_Burn.P_Basic_ForDeathSmoke_Burn'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_BurnDeath(*strPath);
		if (P_BurnDeath.Succeeded())
		{
			P_BurnDeathSmoke = P_BurnDeath.Object;
		}

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Basic/P_Basic_ForDeathSmoke_Poision.P_Basic_ForDeathSmoke_Poision'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_PoisionDeath(*strPath);
		if (P_PoisionDeath.Succeeded())
		{
			P_PoisionDeathSmoke = P_PoisionDeath.Object;
		}
	}

	#pragma region Monster Info Setting

	//# ���� ü�� ���·� �������־�� ��.
	MonsterInfo.MaxHP = 200.0f;
	MonsterInfo.CurrentHP = 200.0f;
	MonsterInfo.Name = FName(L"Basic");
	//MonsterInfo.InfoConditionDataArray.Init(nullptr, 5);

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
}

void ACHM_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@Death (�簡 �Ǿ� �������..) - OnDeathDelegate ���� Mesh Change
	{
		if (bDeath == true)
		{
			//InsertTimer += DeltaTime;
			//if (bInsertForDeathMesh == false && InsertTimer > 3.0f)
			//{
			//	bInsertForDeathMesh = true;

			//	GetIHitComp()->SettingCustomCharactorMesh(ECharactorMeshSort::FORDEATH);

			//	//@�׾ �簡 �Ǵ� Material, Dynamic ���� ����ֱ�
			//	int32 MatCount = GetMesh()->GetNumMaterials();
			//	for (int i = 0; i < MatCount; ++i)
			//	{
			//		UMaterialInterface* MInst = GetMesh()->GetMaterial(i);
			//		if (MInst != nullptr)
			//		{
			//			CLog::Print(MInst->GetName());
			//			UMaterialInstanceDynamic* TargetMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this->GetWorld(), MInst);
			//			if (TargetMat != nullptr)
			//			{
			//				//CLog::Print(TargetMat->GetName());

			//				GetMesh()->SetMaterial(i, TargetMat); //@Set
			//				MatInstDynamicArray.Add(TargetMat);
			//			}
			//		}
			//	}
			//}//(bInsertForDeathMesh)

			for (UMaterialInstanceDynamic* Inst : MatInstDynamicArray)
			{
				FMaterialParameterInfo MatInfo;
				MatInfo.Name = "Appearance";
				float OutValue = 0.0f;
				Inst->GetScalarParameterValue(MatInfo, OutValue);

				(OutValue > 0.0f)
					? OutValue -= 0.01f * DeltaTime * MatDynamicValueSpeed
					: OutValue -= 0.0f;
				Inst->SetScalarParameterValue(FName("Appearance"), OutValue);
			}//(MatinstDynamicArray)

		}//(bDeath == true)
	}
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
		Insert.TintSlateColor = FSlateColor(ConditionType->TintColor);
		Insert.ApplyTime = ConditionType->ApplyTime;

		OutArray->Emplace(Insert);
	}
}

void ACHM_Basic::OnDeath()
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

	IfFalseRetResult(CanBeDamaged(), MonsterInfo.CurrentHP);
	IfTrueRetResult(bDeath == true, MonsterInfo.CurrentHP);

	//CLog::Print(EventInstigator->GetOwner()->GetName());
	//APawn* TestPawn = Cast<APawn>(EventInstigator->GetOwner());
	//if (TestPawn != nullptr)
	//{
	//	CLog::Print(TestPawn->GetName());
	//}

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
		}//(IC_Player)
	}

	MonsterInfo.CurrentHP -= DamageAmount;

	if (MonsterInfo.CurrentHP <= 0.0f)
	{
		CheckDamageTypeForDeath();
		OnDeath();
	}
	return MonsterInfo.CurrentHP;
}

void ACHM_Basic::OnDelegateCharactorDestroy()
{
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACHM_Basic::CallDestory, DeathCallFunctionTimer);
}

void ACHM_Basic::CallDestory()
{
	Destroy();
}

void ACHM_Basic::CheckDamageTypeForDeath()
{
	//@Burn �̳� Poision �� ����Ǿ� �ִٸ�,
	//�� ��, �� �� ����Ǿ��ִٸ�?? -> ���߿� ����Ǿ��ִ� ������..
	//�� �� ���� �� ���� ����
	int BurnTypeIndex = -1, PoisionTypeIndex = -1;
	bool bApplyMatInstDynamic = false;
	FLinearColor InsertColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor BurnColor = FLinearColor(0.88f, 0.072f, 0.0f, 0.0f);
	FLinearColor PoisionColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.0f);

	bool bExistBurn = GetIHitComp()->GetApplyUpsetStateInContainer(EHitUpset::BURN, BurnTypeIndex);
	if (bExistBurn == true)
	{
		bApplyMatInstDynamic = true;
		InsertColor = FLinearColor(0.88f, 0.072f, 0.0f, 0.0f);
	}

	bool bExistPoision = GetIHitComp()->GetApplyUpsetStateInContainer(EHitUpset::POISION, PoisionTypeIndex);
	if (bExistPoision == true)
	{
		bApplyMatInstDynamic = true;
		if (PoisionTypeIndex > BurnTypeIndex)
		{
			InsertColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.0f);
		}
	}
	//@Doing
	if (bApplyMatInstDynamic == true)
	{
		InsertMatInstDynamic(ECharactorMeshSort::FORDEATH, InsertColor);

		UParticleSystem* DoingParticle = nullptr;
		check(P_BurnDeathSmoke);
		check(P_PoisionDeathSmoke);
		(InsertColor == BurnColor)
			? DoingParticle = P_BurnDeathSmoke
			: DoingParticle = P_PoisionDeathSmoke;

		UWorld* const World = GetWorld();
		FVector P_Location = GetActorLocation();

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([DoingParticle, World, P_Location]()
		{
			//@���� �� ����(Smoke) ����.
			FTransform P_Transform;
			P_Transform.SetLocation(P_Location);
			P_Transform.SetScale3D(FVector(4.0f));
			if (DoingParticle != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, DoingParticle, P_Transform, true);
			}
		});

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.0f, false);
	}
}

/*
@Burn ���� �׾��� ��, Color - (0.78f, 0.072f, 0.0f, 0.0f)
@Poision ���� �׾��� ��, Color - ...
*/
void ACHM_Basic::InsertMatInstDynamic(const ECharactorMeshSort Sort, FLinearColor Color)
{
	GetIHitComp()->SettingCustomCharactorMesh(Sort);

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
				//@Set Color
				TargetMat->SetVectorParameterValue(FName("Color"), Color);

				//@Set Material
				GetMesh()->SetMaterial(i, TargetMat); //@Set
				MatInstDynamicArray.Add(TargetMat);
			}
		}
	}
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
