#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Actor/Base/CBaseProjectile.h"

#include "CN_SpawnProjectile.generated.h"

/*
클래스 설명
@해당 SkeletalMesh 에 Location 을 기준으로 둠.
@또한 SpawnParameter 의 Owner 는 SkeletalMesh 를 가지고 있는 Actor 로 설정

*/

UCLASS()
class UE_DOITPROJECT_API UCN_SpawnProjectile
	: public UAnimNotify
{
	GENERATED_BODY()

	#pragma region Reflection
public:
	UPROPERTY(EditAnywhere, Category = "Actor")
	TSubclassOf<class ACBaseProjectile> SpawnProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Value")
	float SpeedValue = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Value")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FVector ScaleOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FName AttachName;

	UFUNCTION()
	void SetProjectileDirection(FVector Dir);

	UFUNCTION()
	void SetProjectileTarget(AActor* Actor);

	#pragma endregion
	
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	FVector Direction = FVector(0.0f);
	AActor* Target;
};
