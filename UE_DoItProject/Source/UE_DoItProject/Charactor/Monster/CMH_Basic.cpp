#include "CMH_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AI/Controller/CAIC_MH_Basic.h"

ACMH_Basic::ACMH_Basic()
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
}

void ACMH_Basic::BeginPlay()
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

		OnCharactorDestroy.AddUObject(this, &ACMH_Basic::OnDestroy);
	}
}

void ACMH_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACMH_Basic::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
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

void ACMH_Basic::OnDestroy()
{
	Destroy();
}
