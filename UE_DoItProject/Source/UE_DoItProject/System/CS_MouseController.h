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
		/* Ex) Player 가 스킬을 쓸 때, 범위를 나타낼 Decal */
		class ACDecalActor_WithMouse* DecalActor;

	UPROPERTY(EditAnywhere, Category = "Actor")
		/*
		DecalActor 가 HitResult 를 구할 객체의 이름의 일부분.
		Ex) Floor가 들어간 이름(ex.바닥) 에 사용
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
	@param - DecalCircleRange MouseController 안에 있는 Decal Size 지정
	@param - StandardTarget Unproject 의 기준점이 되는 Target 설정 Default = nullptr
	@param - StandardRange 기준점의 반경 Default = 0.0f
	*/
	void OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);
	void OffUsingDecalMouseControl();

	//Temp(임시)
	//void SettingDecalMaterial();

private:
	/* Ex) Mouse 클릭 시 Monster 타겟 설정 */
	bool TargetMouseRayAndHit(APlayerController* PC, ECollisionChannel Channel);

	/* Ex) 스킬 사용 시 범위 설정 */
	bool UsingDecalMouseRayAndHit(APlayerController* PC, FVector& HitedLocation, ECollisionChannel Channel);

	/* Member */
public:
	EMouseState GetMouseState() const { return MouseState; }
	FVector GetClickPoint() const { return ClickPoint; }


private:
	class ACPlayer* Player;

	int32 ControllerIndex = 0;
	bool bUsingDecalMouseControl = false;

	bool bDebug = false;

	ECollisionChannel MouseCollisionChannel = ECollisionChannel::ECC_Visibility;
	EMouseState MouseState = EMouseState::NONE; //@마우스 상태
	FVector ClickPoint = FVector(0.0f); //@LeftButton 클릭 시, 그 위치

	/* DecalActor 의 원 반경 */
	FVector DecalActorCircleSize = FVector(200.0f);

	/* Mouse Deprojection 반경 */
	AActor* TargetMouseStandard = nullptr;
	float MouseRangeWithTarget = 0.0f;

	/* Target Info - (Widget) */
	class UWG_TargetInfo* TargetInfoWidget;
};
