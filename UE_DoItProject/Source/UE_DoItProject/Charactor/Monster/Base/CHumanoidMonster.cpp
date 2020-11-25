#include "CHumanoidMonster.h"
#include "Global.h"

#include "_GameInst/CGameInst.h"
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

	FString strPath = L"";

	//@LOAD
	{
		strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_FloatingCombo.BpCWG_FloatingCombo_C'";
		ConstructorHelpers::FClassFinder<UWG_FloatingCombo> LoadComboUIClass(*strPath);
		if (LoadComboUIClass.Succeeded())
		{
			FloatingComboClass = LoadComboUIClass.Class;
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

void ACHumanoidMonster::AddCurrentHP(float fValue)
{
}

void ACHumanoidMonster::AddCurrentMP(float fValue)
{
}

void ACHumanoidMonster::AddATK(float fValue)
{
}

void ACHumanoidMonster::AddDEF(float fValue)
{
}
