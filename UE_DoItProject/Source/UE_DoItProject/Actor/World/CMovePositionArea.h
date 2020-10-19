#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMovePositionArea.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACMovePositionArea 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
public:
	UPROPERTY(EditAnywhere, Category = "MapData")
		bool bStartingPoint = false;

	UPROPERTY(EditAnywhere, Category = "MapData")
		class ACMovePositionArea* MovingAreaToActor = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UTextRenderComponent* TextRender;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_WarpDisplay;

	UPROPERTY(EditAnywhere, Category = "Text")
		float TextRotationSpeed = 30.0f;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

	
public:	
	ACMovePositionArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void BeginMove();

	/* Member */
private:

};
