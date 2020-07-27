#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Figure/CPlaneActor.h"

#include "CSkillRangeDisplay.generated.h"

UENUM()
enum class ESortType
{
	BACK = 0,
	FWD = 1,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDelOverlapActorForSkill, AActor*)

UCLASS()
class UE_DOITPROJECT_API ACSkillRangeDisplay 
	: public AActor
{
	GENERATED_BODY()
	
#pragma	region Reflection
public:
	/* ��ų ���� Overlap �� �� ���� �� */
	FDelOverlapActorForSkill	OnDelOverlapSkillRange;

	/* 'SkillTimer' �ð� �� ����� */
	FTimerDelegate				OnDelEndSkillCall;


private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* BackGroundStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* ForwardStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* PTComp;

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
	ACSkillRangeDisplay();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void SetVisibility(bool bValue);

	//@Warning -
	//@param Owner - �ش� ��ų�� �����ϴ� ����
	//@param fFillTimer - ���� ä�������ϴ� �ð�
	//@param fSkillTimer - (���� ä������ ��) ��ų�� ������ �ð�
	void FillStart(AActor* SettingOwner, float fFillTimer, float fSkillTimer);

	void SetDecalCompMat(class UMaterialInterface* Material, ESortType Type);
	void SetBackGroundDecalSize(float fSize);
	void SetBackGroundDecalSize(FVector2D vSize);
	void SetCollisionBoxExtent(FVector2D vSize);
	void SetParticleSystem(class UParticleSystem* PT);
	void SetParticleSystem(class UParticleSystem* PT, const FTransform& RelativeTransform);


	/* Memeber */
private:
	float DefaultBoxHeight = 1000.0f;
};
