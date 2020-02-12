#include "CHM_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AI/Controller/CAIC_HM_Basic.h"

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
		AttackComp = CreateDefaultSubobject<UCHM_BasicAttackComp>("AttackComp");
		EquipComp = CreateDefaultSubobject<UCHM_BasicEquipComp>("EquipComponent");
	}
}

void ACHM_Basic::BeginPlay()
{
	Super::BeginPlay();

	// Set Delegate
	{
		OnDeath.AddLambda([&]()
		{
			SetActorEnableCollision(false);
			//AutoPossessAI = EAutoPossessAI::Disabled;
			//AIControllerClass = nullptr;
		});

		OnCharactorDestroy.AddUObject(this, &ACHM_Basic::OnDestroy);
	}
}

void ACHM_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACHM_Basic::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
{
	check(Montage);

	if (bAlways == false)
	{
		// 만약 캐릭터가 몽타주 실행 중이라면,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}

IIC_AttackComp * ACHM_Basic::GetIAttackComp()
{
	IfTureRetResult(AttackComp == nullptr, nullptr); // @Return Null

	return Cast<IIC_AttackComp>(AttackComp);
}

IIC_EquipComp * ACHM_Basic::GetIEquipComp()
{
	IfTureRetResult(EquipComp == nullptr, nullptr); // @Return Null

	return Cast<IIC_EquipComp>(EquipComp);
}

void ACHM_Basic::OnDestroy()
{
	Destroy();
}
