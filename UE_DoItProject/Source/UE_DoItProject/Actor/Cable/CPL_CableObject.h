#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPL_CableObject.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPL_CableObject 
	: public AActor
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Material")
		class UMaterialInterface* OnMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Cable")
	class UCableComponent* CableComp;

	UPROPERTY(EditAnyWhere, Category = "Cable")
		FString AttachName = L"hand_l";

	//UFUNCTION(Category = "CableOverlapFunc")
	//	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//UFUNCTION(Category = "CableOverlapFunc")
	//	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

public:	
	ACPL_CableObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	FString GetAttachName() const { return AttachName; }
	void SetPullStartTarget(AActor* Target);
	void ResetState();


	#pragma	region Member
public:
	bool GetStretching() const { return bStretch; }
	bool GetPulling() const { return bPulling; }

private:
	class ACPlayer* Player;
	AActor* PullTarget;
	bool bStretch			= false;
	bool bPulling			= false;

	// Value
	float UsingDeltaTime = 0.0f;
	float StretchTime = 0.3f;
	float InterpSpeed = 3.0f;

	#pragma endregion
};
