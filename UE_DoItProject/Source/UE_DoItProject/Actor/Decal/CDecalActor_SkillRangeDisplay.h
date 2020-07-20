#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Figure/CPlaneActor.h"

#include "CDecalActor_SkillRangeDisplay.generated.h"

UENUM()
enum class ESortType
{
	BACK = 0,
	FWD = 1,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDelOverlapActorForSkill, AActor*)

UCLASS()
class UE_DOITPROJECT_API ACDecalActor_SkillRangeDisplay 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
		/* Skill 기능을 받을 Delegate */
public:
	FDelOverlapActorForSkill OnDelOverlapSkillRange;

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* BackGroundStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* ForwardStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* Mat_BackGround;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* Mat_Forward;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(EditAnywhere, Category = "Value")
	/* 원이 채워져야하는 시간 설정 */
	float FillTimer = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Value")
	/* 스킬이 시전될 시간 설정 */
	float SkillTimer = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool bFilling = false;

	UPROPERTY(EditAnywhere, Category = "Value")
	float FillingSpeed = 1.0f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion
	
public:	
	ACDecalActor_SkillRangeDisplay();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	//@Warning -
	//@param Owner - 해당 스킬을 시전하는 액터
	//@param fFillTimer - 원이 채워져야하는 시간
	//@param fSkillTimer - (원이 채워지고난 후) 스킬이 시전될 시간
	void FillStart(AActor* Owner, float fFillTimer, float fSkillTimer);

	void SetDecalCompMat(class UMaterialInterface* Material, ESortType Type);
	void SetBackGroundDecalSize(float fSize);
	void SetBackGroundDecalSize(FVector2D vSize);
	void SetCollisionBoxExtent(FVector2D vSize);

	/* Memeber */
private:
	float DefaultBoxHeight = 1000.0f;
};
