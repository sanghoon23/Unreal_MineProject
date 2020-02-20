#include "CHumanoidMonster.h"
#include "Global.h"

#include "AI/Controller/CAIC_HM_Basic.h"

ACHumanoidMonster::ACHumanoidMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set AI
	FString path = L"";
	{
		path = L"Blueprint'/Game/_Mine/_MyBlueprint/AI/Controller/BpCAIC_HM_Basic.BpCAIC_HM_Basic_C'";
		ConstructorHelpers::FClassFinder<ACAIC_HM_Basic> controller(*path);
		if (controller.Succeeded())
		{
			MonAIControllerClass = controller.Class;
			AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
			AIControllerClass = MonAIControllerClass;
		}
	}

}

void ACHumanoidMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACHumanoidMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHumanoidMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACHumanoidMonster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

