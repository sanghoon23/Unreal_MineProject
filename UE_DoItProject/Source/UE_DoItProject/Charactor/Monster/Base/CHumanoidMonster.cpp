#include "CHumanoidMonster.h"
#include "Global.h"

#include "AIController.h"

ACHumanoidMonster::ACHumanoidMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	//@AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	////@Create
	//{
	//	CustomMovementComp = CreateDefaultSubobject<UCCharactorMovementComp>("CustomMovementComp");
	//	CustomMovementComp->UpdatedComponent = RootComponent;
	//}

	//CharacterMovementComponentName = "CustomMovementComp";
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

//UPawnMovementComponent* ACHumanoidMonster::GetMovementComponent() const
//{
//	return CustomMovementComp;
//}
