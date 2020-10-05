#include "CAIC_BaseHM.h"
#include "Kismet/KismetMathLibrary.h"
#include "CLog.h"

#include "Interface/IC_BaseAttack.h"
#include "State/Base/C_BaseAttackState.h"

ACAIC_BaseHM::ACAIC_BaseHM()
{
}

void ACAIC_BaseHM::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIC_BaseHM::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
}

void ACAIC_BaseHM::OnUnPossess()
{
	Super::OnUnPossess();
}

int ACAIC_BaseHM::GetTypePatternAttackState()
{
	if (PatternAttackTypeList.Num() == 0) return -1;

	int RandomValue 
		= UKismetMathLibrary::RandomIntegerInRange(0, PatternAttackTypeList.Num() - 1);

	return PatternAttackTypeList[RandomValue];
}

int ACAIC_BaseHM::GetTypeRandomAttackState()
{
	if (RandomAttackTypeList.Num() == 0) return -1;

	uint8 RandomValue
		= UKismetMathLibrary::RandomIntegerInRange(0, RandomAttackTypeList.Num() - 1);

	return RandomAttackTypeList[RandomValue];
}
