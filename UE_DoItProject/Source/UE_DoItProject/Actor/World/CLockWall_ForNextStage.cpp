#include "CLockWall_ForNextStage.h"
#include "Global.h"

#include "_GameInst/CGameInst.h"

ACLockWall_ForNextStage::ACLockWall_ForNextStage()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(SceneComp);

	//Setting
	{
		BoxComp->SetBoxExtent(FVector(100.0f, 10.0f, 100.0f));
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetCollisionProfileName("InvisibleWall");
		BoxComp->SetGenerateOverlapEvents(true);
	}
}

void ACLockWall_ForNextStage::BeginPlay()
{
	Super::BeginPlay();
}

void ACLockWall_ForNextStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACLockWall_ForNextStage::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CLog::Print(L"LockWall TakeDamage!!");

	return Damage;
}
