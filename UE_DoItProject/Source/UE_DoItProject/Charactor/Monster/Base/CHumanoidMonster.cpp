#include "CHumanoidMonster.h"
#include "Global.h"

#include "AIController.h"
#include "UI/Widget/WG_FloatingCombo.h"

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

	UWorld* const World = GetWorld();

	FVector InsertPos = GetActorLocation();

	UWG_FloatingCombo* FloatingComboUI = CreateWidget<UWG_FloatingCombo>(GetWorld(), FloatingComboClass);
	if (FloatingComboUI != nullptr)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@ÁÖÃ¼ÀÚ.
		if (PC != nullptr)
		{
			FloatingComboUI->SetInitial(PC, InsertPos, EFloatingComboColor::WHITE);
			FloatingComboUI->SetDisplayDamageValue(DamageAmount);

			FloatingComboUI->AddToViewport();
		}

		//CLog::Print(L"Spawn FloatingComboUI !!");
	}

	return Damage;
}
