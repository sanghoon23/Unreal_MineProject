#include "CHM_ShamanFirstAttack.h"
#include "Global.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Shaman.h"
#include "Actor/Particle/UseMonster/CParticle_Lighting.h"

UCHM_ShamanFirstAttack::UCHM_ShamanFirstAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	FString Path = L"";

	//@LOAD Particle
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Shaman/Shaman_FirstAttack_Casting.Shaman_FirstAttack_Casting'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_LightingCast(*Path);
		if (P_LightingCast.Succeeded())
		{
			ParticleLightingCast = P_LightingCast.Object;
		}
	}

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Shaman_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Attack/HM_Shaman_Mon_FirstAttack.HM_Shaman_Mon_FirstAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Shaman_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Shaman_FirstAttack);
	}

#pragma endregion

}

void UCHM_ShamanFirstAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Shaman = Cast<ACHM_Shaman>(GetOwner());
	check(HM_Shaman);

	#pragma region Spawn Particle Object

	FTransform Transform = FTransform::Identity;
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	LightingActor = GetWorld()->SpawnActor<ACParticle_Lighting>(ACParticle_Lighting::StaticClass(), Transform, Params);
	LightingActor->OffEndActor();

	DT_Stun = NewObject<UCDamageType_Stun>();
	DT_Stun->SetDamageImpulse(10.0f);
	DT_Stun->SetStunTime(1.5f);
	LightingActor->SetDamageType(DT_Stun);

	#pragma endregion

	// @EndAttack Delegate
	EndAttackDeleFunc.AddLambda([&]()
	{
		//@Off Particle Object
		LightingActor->OffEndActor();
	});

}

void UCHM_ShamanFirstAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* 이 공격이 실행된다면 IsRunTick 에 의해서 Tick 이 호출되므로 항상 bAttacking == true */
	if (bAttacking == true)
	{
		FName CurrentSectionName = HM_Shaman->GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(AttackMontages[0]);
		if (CurrentSectionName == SectionStartName)
		{
			//Particle
			UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached
			(
				ParticleLightingCast,
				HM_Shaman->GetMesh(),
				FName("hand_r"),
				FVector(0.0f), FRotator(0.0f), EAttachLocation::SnapToTarget
			);

			ParticleComp->SetRelativeScale3D(FVector(0.3f));
		}
		else
		{
			//@On Particle Object
			AActor* TargetInAI = HM_Shaman->GetTargetInAI();
			if (TargetInAI != nullptr)
			{
				LightingActor->OnStartActor
				(
					TargetInAI->GetActorLocation()
				);

				FTransform Transform;
				Transform.SetLocation(FVector(0.0f, 0.0f, -50.0f));
				Transform.SetRotation(FQuat(FRotator(0.0f)));
				Transform.SetScale3D(FVector(1.5f));
				LightingActor->SetParticleCompRelative(Transform);
			}
		}
	}
}

void UCHM_ShamanFirstAttack::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_ShamanFirstAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	ACHM_Shaman* Pawn = Cast<ACHM_Shaman>(DoingActor);
	check(Pawn);

	// @IF TRUE RETURN
	IfTrueRet(Pawn->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Pawn->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	if (bAttacking == false)
	{
		Pawn->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}

