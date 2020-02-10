#include "CMonsterHumanoid.h"
#include "Global.h"

#include "AI/Controller/CAIC_MH_Basic.h"

ACMonsterHumanoid::ACMonsterHumanoid()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set AI
	FString path = L"";
	{
		path = L"Blueprint'/Game/_Mine/_MyBlueprint/AI/Controller/BpCAIC_MH_Basic.BpCAIC_MH_Basic_C'";
		ConstructorHelpers::FClassFinder<ACAIC_MH_Basic> controller(*path);
		if (controller.Succeeded())
		{
			MonAIControllerClass = controller.Class;
			AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
			AIControllerClass = MonAIControllerClass;
		}
	}

}

void ACMonsterHumanoid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACMonsterHumanoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACMonsterHumanoid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACMonsterHumanoid::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

