#include "CS_AttackDecision.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCS_AttackDecision::UCS_AttackDecision()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCS_AttackDecision::BeginPlay()
{
	Super::BeginPlay();
}

void UCS_AttackDecision::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAutoAttackCall == true)
	{
		// @AttackRange ���� ũ��, Controller Input Vec
		if (SettedCharactor->GetHorizontalDistanceTo(Target) > AttackRange)
		{
			FVector MoveDir = Target->GetActorLocation() - SettedCharactor->GetActorLocation();
			MoveDir.Z = 0.0f;
			SettedCharactor->GetCharacterMovement()->AddInputVector(MoveDir, 1.0f);
		}
		else
		{
			bAutoAttackCall = false;
			bAttackPossible = true;
		}
	}
}

void UCS_AttackDecision::OnAble(ACharacter * OwnerCharactor, float AtkRange)
{
	check(OwnerCharactor);

	Attackable = EAutoAttackable::USE;
	SettedCharactor = OwnerCharactor;
	bAttackPossible = false;

	AttackRange = AtkRange;
}

/* ������� ���� ������ bAttackPossible �� true ���� ( Default ) */
void UCS_AttackDecision::UnAble()
{
	Attackable = EAutoAttackable::UN_USE;
	bAttackPossible = true;
}

//@Warning - bAttackPossible ������ Tick ���� ����
void UCS_AttackDecision::StartAttackTrace(AActor * TraceTarget)
{
	check(TraceTarget);
	Target = TraceTarget;
	bAutoAttackCall = true;

	bAttackPossible = false;
}

//@Warning - bAttackPossible ������ Tick ���� ����
void UCS_AttackDecision::StopAttackTrace()
{
	Target = nullptr;
	bAutoAttackCall = false;

	bAttackPossible = false;
}
