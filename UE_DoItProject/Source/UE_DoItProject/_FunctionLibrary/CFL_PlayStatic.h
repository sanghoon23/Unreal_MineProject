#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFL_PlayStatic.generated.h"

/*
#Edit - 0423
@PlayStatic Obejct
* Gameplay 구현에 있어서 자주 쓰는 함수들을 위해 전역적으로 만든 클래스

*/

UCLASS()
class UE_DOITPROJECT_API UCFL_PlayStatic 
	: public UObject
{
	GENERATED_BODY()
	
public:
	UCFL_PlayStatic();

public:
	/* 보류 */
	//UFUNCTION(Category = "PlayStatic")
	//static bool IsInputEKey(APlayerController* PC, TArray<EKeys> Keys);

};
