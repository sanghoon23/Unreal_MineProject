#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"

#include "CFL_ActorAgainst.generated.h"

/*
#Edit - 0410
@ActorAgainst Obejct
* Target ���� ��ȣ�ۿ��� ���� ���������� ���� Ŭ����

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
	/* Subject �� Target �տ� Distance ��ŭ ���� ���� - ���̵� ���� */
	//@param HeightInclude - ���̵� ������ ���ΰ�
	static void ActorLocateFrontTarget(AActor* Target, AActor* Subject, float Distance, bool HeightInclude);

	UFUNCTION(Category = "ActorAgainst")
	/* Controller �������� ȸ�� */
	//@param Subject - ACharacter
	//@param Angle - Character �� �������� Controller �� ȸ���� ����
	static void SetAngleWithControlRot(ACharacter* Subject, float Angle);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//#Edit0407 - APawn �ޱ� ���ؼ� �����ε�
	//@param Target - APawn
	//UFUNCTION(Category = "ActorAgainst")
	//	static bool IsTargetInAir(APawn* Target);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(Category = "ActorAgainst")
	// ���� ACharacter �� ���� üũ ����
	/* Target �� ���߿� �ִ��� �ƴ��� �Ǻ� */
	//@param Target - ACharacter
	//@Warning - IsFalling() �����ؼ� Gravity <= 0.0f �� ���� true
	static bool IsTargetInAir(ACharacter* Target);

};
