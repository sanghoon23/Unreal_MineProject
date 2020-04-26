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
	@param - DecalCircleRange MouseController 안에 있는 Decal Size 지정
	@param - StandardTarget Unproject 의 기준점이 되는 Target 설정 Default = nullptr
	@param - StandardRange 기준점의 반경 Default = 0.0f
	*/
	void OnMouseControl(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffMouseControl();

	//Temp(임시)
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

	/* DecalActor 의 원 반경 */
	FVector DecalActorCircleSize = FVector(300.0f);

	/* Mouse Deprojection 반경 */
	AActor* TargetMouseStandard = nullptr;
	float MouseRangeWithTarget = 0.0f;
};
