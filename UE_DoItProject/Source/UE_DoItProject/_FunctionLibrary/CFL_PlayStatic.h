#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFL_PlayStatic.generated.h"

/*
#Edit - 0423
@PlayStatic Obejct
* Gameplay ������ �־ ���� ���� �Լ����� ���� ���������� ���� Ŭ����

*/

UCLASS()
class UE_DOITPROJECT_API UCFL_PlayStatic 
	: public UObject
{
	GENERATED_BODY()
	
public:
	UCFL_PlayStatic();

public:
	/* ���� */
	//UFUNCTION(Category = "PlayStatic")
	//static bool IsInputEKey(APlayerController* PC, TArray<EKeys> Keys);

};
