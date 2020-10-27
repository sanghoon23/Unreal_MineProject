#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_MouseController.generated.h"

UENUM()
enum class EMouseState : uint8
{
	NONE = 0,
	WAIT = 1,
	CHECKINGPOINT = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCS_MouseController 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Actor")
		/* Ex) Player �� ��ų�� �� ��, ������ ��Ÿ�� Decal */
		class ACDecalActor_WithMouse* DecalActor;

	UPROPERTY(EditAnywhere, Category = "Actor")
		/*
		DecalActor �� HitResult �� ���� ��ü�� �̸��� �Ϻκ�.
		Ex) Floor�� �� �̸�(ex.�ٴ�) �� ���
		*/
		FString HitFloorContainName = "Floor";

	UPROPERTY(EditAnywhere, Category = "Material")
		class UMaterialInterface* DecalMatCanUsingRange;

	UPROPERTY(EditAnywhere, Category = "Material")
		class UMaterialInterface* DecalMatCanNotUsingRange;

	#pragma endregion

public:	
	UCS_MouseController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	/*
	@param - DecalCircleRange MouseController �ȿ� �ִ� Decal Size ����
	@param - StandardTarget Unproject �� �������� �Ǵ� Target ���� Default = nullptr
	@param - StandardRange �������� �ݰ� Default = 0.0f
	*/
	void OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffUsingDecalMouseControl();

	//Temp(�ӽ�)
	//void SettingDecalMaterial();

private:
	/* Ex) Mouse Ŭ�� �� Monster Ÿ�� ���� */
	bool TargetMouseRayAndHit(APlayerController* PC);

	/* Ex) ��ų ��� �� ���� ���� */
	bool UsingDecalMouseRayAndHit(APlayerController* PC, FVector& HitedLocation);

	/* Member */
public:
	EMouseState GetMouseState() const { return MouseState; }
	FVector GetClickPoint() const { return ClickPoint; }


private:
	class ACPlayer* Player;

	int32 ControllerIndex = 0;
	bool bUsingDecalMouseControl = false;

	bool bDebug = true;
	EMouseState MouseState = EMouseState::NONE;
	FVector ClickPoint = FVector(0.0f);

	/* DecalActor �� �� �ݰ� */
	FVector DecalActorCircleSize = FVector(200.0f);

	/* Mouse Deprojection �ݰ� */
	AActor* TargetMouseStandard = nullptr;
	float MouseRangeWithTarget = 0.0f;

	/* Target Info - (Widget) */
	class UWG_TargetInfo* TargetInfoWidget;
};
