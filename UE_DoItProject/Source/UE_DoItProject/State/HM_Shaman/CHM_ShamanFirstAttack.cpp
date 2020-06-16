#include "CHM_ShamanFirstAttack.h"
#include "Global.h"
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

		AttackRange = 1000.0f;
	}

#pragma region FirstAttack

	FString Path = L"";
	// Setting
	{
		UAnimMontage* Shaman_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Attack/ShamanMon_FirstAttack.ShamanMon_FirstAttack'";
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
}

//TODO : Shaman 마법 번개 완성하기, 데미지타입 - 기본, 스턴
void UCHM_ShamanFirstAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	ACHM_Shaman* Pawn = Cast<ACHM_Shaman>(DoingActor);
	check(Pawn);

	// @IF TRUE RETURN
	IfTrueRet(Pawn->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Pawn->GetIEquipComp()->GetEquiping()); //@Equping Check

	//@IsLastCombo - 
	// AI 에서 BeginAttack 을 계속 호출하기 때문에,
	// Player 랑 다르게 제어가 필요하다.
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

