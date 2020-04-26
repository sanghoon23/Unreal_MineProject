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
		class ACDecalActor_WithMouse* DecalActor;

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
	void OnMouseControl(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffMouseControl();

	//Temp(�ӽ�)
	//void SettingDecalMaterial();

private:
	bool MouseRayAndHit(APlayerController* PC, FVector& HitedLocation);

	/* Member */
public:
	EMouseState GetMouseState() const { return MouseState; }
	FVector GetClickPoint() const { return ClickPoint; }


private:
	int32 ControllerIndex = 0;
	bool bUsingControl = false;

	bool bDebug = false;
	EMouseState MouseState = EMouseState::NONE;
	FVector ClickPoint = FVector(0.0f);

	/* DecalActor �� �� �ݰ� */
	FVector DecalActorCircleSize = FVector(300.0f);

	/* Mouse Deprojection �ݰ� */
	AActor* TargetMouseStandard = nullptr;
	float MouseRangeWithTarget = 0.0f;
};
