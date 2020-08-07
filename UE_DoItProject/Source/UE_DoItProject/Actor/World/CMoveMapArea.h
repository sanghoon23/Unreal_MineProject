#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMoveMapArea.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACMoveMapArea 
	: public AActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_WarpDisplay;

	UPROPERTY(EditAnywhere, Category = "MapData")
		FName LoadMapName = "";

	UPROPERTY(EditAnywhere, Category = "Component")
		TSubclassOf<class UUserWidget> LoadingBackgroundClass;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

	
public:	
	ACMoveMapArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
