#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"

#include "CFL_ActorAgainst.generated.h"

/*
#Edit - 0410
@ActorAgainst Obejct
* Target 과의 상호작용을 위해 전역적으로 만든 클래스

*/

UCLASS()
class UE_DOITPROJECT_API UCFL_ActorAgainst 
	: public UObject
{
	GENERATED_BODY()

public:
	UCFL_ActorAgainst();

public:
	UFUNCTION(Category = "ActorAgainst")
	static void LookAtTarget(AActor* Subject, AActor* Target);

	UFUNCTION(Category = "ActorAgainst")
	static void LookAtPoint(AActor* Subject, FVector& Point);

	UFUNCTION(Category = "ActorAgainst")
	/* Subject 를 Target 앞에 Distance 만큼 으로 맞춤 - 높이도 포함 */
	//@param HeightInclude - 높이도 포함할 것인가
	static void ActorLocateFrontTarget(AActor* Target, AActor* Subject, float Distance, bool HeightInclude);

	UFUNCTION(Category = "ActorAgainst")
	/* Controller 기준으로 회전 */
	//@param Subject - ACharacter
	//@param Angle - Character 를 기준으로 Controller 가 회전할 방향
	static void SetAngleWithControlRot(ACharacter* Subject, float Angle);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//#Edit0407 - APawn 받기 위해선 오버로딩
	//@param Target - APawn
	//UFUNCTION(Category = "ActorAgainst")
	//	static bool IsTargetInAir(APawn* Target);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(Category = "ActorAgainst")
	// 현재 ACharacter 만 공중 체크 가능
	/* Target 이 공중에 있는지 아닌지 판별 */
	//@param Target - ACharacter
	//@Warning - IsFalling() 포함해서 Gravity <= 0.0f 일 때도 true
	static bool IsTargetInAir(ACharacter* Target);

};
