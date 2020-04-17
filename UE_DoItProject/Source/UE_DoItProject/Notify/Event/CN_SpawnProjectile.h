#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Actor/Base/CBaseProjectile.h"

#include "CN_SpawnProjectile.generated.h"

/*
Ŭ���� ����
@�ش� SkeletalMesh �� Location �� �������� ��.
@���� SpawnParameter �� Owner �� SkeletalMesh �� ������ �ִ� Actor �� ����

* ���� BaseProjectile �� ����� Actor �� ���� Spawn ��ų����
* "��� �߰�" ���־�� ��.

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
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FVector ScaleOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
	FName AttachName;

	#pragma endregion
	
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
