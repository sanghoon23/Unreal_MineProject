#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "CDM_MeshActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACDM_MeshActor
	: public ADestructibleActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
public:
	UPROPERTY(BlueprintReadWrite, Category = "HitOut")
		/* �´� ��󿡼� ���ܵ� Actor �̸� */
		TArray<FString> HitOutString;

	UPROPERTY(BlueprintReadWrite, Category = "HitOut")
		float HitAfterLifeTime = 3.0f;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

private:
	//UFUNCTION()
	//	virtual void OnFracture(const FVector& HitPoint, const FVector& HitDirection);

	#pragma endregion

	
public:	
	ACDM_MeshActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* Function  */
protected:
	void Death();
};
