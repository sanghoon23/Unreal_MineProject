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
		/* Skill ����� ���� Delegate */
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
	/* ���� ä�������ϴ� �ð� ���� */
	float FillTimer = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Value")
	/* ��ų�� ������ �ð� ���� */
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
	//@param Owner - �ش� ��ų�� �����ϴ� ����
	//@param fFillTimer - ���� ä�������ϴ� �ð�
	//@param fSkillTimer - (���� ä������ ��) ��ų�� ������ �ð�
	void FillStart(AActor* Owner, float fFillTimer, float fSkillTimer);

	void SetDecalCompMat(class UMaterialInterface* Material, ESortType Type);
	void SetBackGroundDecalSize(float fSize);
	void SetBackGroundDecalSize(FVector2D vSize);
	void SetCollisionBoxExtent(FVector2D vSize);

	/* Memeber */
private:
	float DefaultBoxHeight = 1000.0f;
};
