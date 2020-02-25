#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_InteractActor.generated.h"

UENUM()
enum class InteractType : uint8
{
	JUMPOVER = 0,
	CLIMB = 1,
	END = 2,
};

/* ��ȣ�ۿ�(Interactive) �� ���� Data ���� */
// @Waring -
// Ư�� InteractType ���� �ش� Type ���� ���� ������ Setting ���־�� ��.
USTRUCT()
struct FInteractiveData
{
	GENERATED_USTRUCT_BODY()

	float JumpOverSpeed = 0.0f;

	//...
};

UINTERFACE(MinimalAPI)
class UIC_InteractActor : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_InteractActor
{
	GENERATED_BODY()

	/* Vritual Function */
public:
	// @param Actor - ��ȣ�ۿ� '����' Actor
	virtual void BeginInteract(AActor* InteractedActor) = 0;

	// @param Actor - ��ȣ�ۿ� '����' Actor
	virtual void TickInteract(AActor* InteractedActor) = 0;

	// @param Actor - ��ȣ�ۿ� '����' Actor
	virtual void EndInteract(AActor* InteractedActor) = 0;

public:
	InteractType GetInteractType() { return Type; }
	void SetInteractiveData(const FInteractiveData& FData) { InteractiveData = FData; }

protected:
	InteractType Type = InteractType::END;

	FInteractiveData InteractiveData;
};
