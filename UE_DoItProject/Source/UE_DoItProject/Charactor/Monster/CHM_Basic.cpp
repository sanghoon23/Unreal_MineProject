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
		HitComp = CreateDefaultSubobject<UCHM_BasicHitComp>("HitComp");
		EquipComp = CreateDefaultSubobject<UCHM_BasicEquipComp>("EquipComponent");
	}

	#pragma region Monster Info Setting

	//# 현재 체력 상태로 갱신해주어야 함.
	Info.HP = 50.0f;
	Info.Name = FName(L"HM_Basic");
	Info.InfoConditionDataArray.Init(nullptr, 5);

	#pragma endregion

	
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
	
	//GetMesh()->SetMaterial();

}

void ACHM_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//#WG_TargetInfo 에서 수행.
	#pragma region Monster Info Update

	//for (int i = 0; i < Info.InfoConditionDataArray.Num(); ++i)
	//{
	//	//@Update
	//	HitComp->GetConditionDatas(&(Info.InfoConditionDataArray));
	//}

	#pragma endregion
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

IIC_HitComp * ACHM_Basic::GetIHitComp()
{
	IfTureRetResult(HitComp == nullptr, nullptr); // @Return Null

	return Cast<IIC_HitComp>(HitComp);
}

void ACHM_Basic::OnDestroy()
{
	Destroy();
}
