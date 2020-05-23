#include "CPL_CableObject.h"
#include "Global.h"

#include "CableComponent.h"
#include "Charactor/Player/CPlayer.h"

ACPL_CableObject::ACPL_CableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create
	CableComp = CreateDefaultSubobject<UCableComponent>("HandCableComponent");
	RootComponent = CableComp;

	FString path = L"";
	// Component Setting
	{
		CableComp->SetVisibility(false);
		CableComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CableComp->SetCollisionProfileName("OverlapOnlyPawn");

		path = L"Material'/Game/_Mine/UseMaterial/Cable/Mat_CableObject.Mat_CableObject'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObject(*path);
		if (MaterialObject.Succeeded())
		{
			OnMaterial = MaterialObject.Object;
		}
		//OnMaterial = CreateDefaultSubobject<UMaterialInterface>("OnMaterial");

		CableComp->SetMaterial(0, OnMaterial);
	}

	// Value Setting
	{
		CableComp->bAttachEnd = true;
		CableComp->EndLocation = GetActorLocation(); //@끝지점 위치
		CableComp->bEnableStiffness = true;
		CableComp->bEnableCollision = true;
		CableComp->SetEnableGravity(true);
		CableComp->CableGravityScale = 0.0f;
		CableComp->CableLength = 100.0f;
		CableComp->CableWidth = 5.0f;
		CableComp->NumSides = 10;
	}
}

void ACPL_CableObject::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Player);

	//@BeginOverlap
	//CableComp->OnComponentBeginOverlap.AddDynamic(this, &ACPL_CableObject::OnBeginOverlap);
	//CableComp->OnComponentEndOverlap.AddDynamic(this, &ACPL_CableObject::OnEndOverlap);
}

void ACPL_CableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PullTarget != nullptr && bStretch == true)
	{
		if (UsingDeltaTime < StretchTime)
		{
			UsingDeltaTime += DeltaTime;
			CableComp->CableLength 
				= UKismetMathLibrary::Lerp(CableComp->CableLength, GetDistanceTo(PullTarget) * 0.3f, 0.8f);

			//@Attach To Target
			FComponentReference AttachRef;
			AttachRef.OtherActor = PullTarget;
			CableComp->AttachEndTo = AttachRef;
		}
		else
		{
			UsingDeltaTime = 0.0f;
			bStretch = false;
			bPulling = true;
		}

	}//if(bStretch == true)
	else if (PullTarget != nullptr && bPulling == true)
	{
		CableComp->CableLength
			= UKismetMathLibrary::Lerp(CableComp->CableLength, GetDistanceTo(PullTarget) * 1.0f, 0.7f);

	}//if(bPulling == true)
}

void ACPL_CableObject::SetPullStartTarget(AActor * Target)
{
	bStretch = true;
	PullTarget = Target;

	CableComp->SetVisibility(true);
	UsingDeltaTime = 0.0f;
}

void ACPL_CableObject::ResetState()
{
	//@Value
	bStretch = false;
	bPulling = false;
	UsingDeltaTime = 0.0f;
	PullTarget = nullptr;

	//@Attach Target Null
	FComponentReference AttachRef;
	AttachRef.OtherActor = nullptr;
	CableComp->AttachEndTo = AttachRef;

	//@Cable Length
	CableComp->SetVisibility(false);
	CableComp->CableLength = 100.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void ACPL_CableObject::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	IfNullRet(OverlappedComponent);
//	IfNullRet(OtherActor);
//	IfNullRet(OtherComp);
//}
//
//void ACPL_CableObject::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
//{
//	IfNullRet(OverlappedComponent);
//	IfNullRet(OtherActor);
//	IfNullRet(OtherComp);
//}
