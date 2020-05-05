#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseConditionType.generated.h"

UENUM()
/* ���� �̻� ���� */
enum class EHitUpset : uint8
{
	SLOWMOVE	= 0, //�̵�����
	STUN		= 1, //����
	BURN		= 2, //ȭ��
	POISION		= 3, //�ߵ�
	FREEZE		= 4, //����
	SLEEP		= 5, //����
};

UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UCBaseConditionType 
	: public UObject
{
	GENERATED_BODY()
public:
	UCBaseConditionType() {};

	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* @true - LeftToRight, @false - RightToLeft*/
		bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo ǥ�� Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* ���� ���� �ð� */
		float ApplyTime = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Data")
		EHitUpset State;

	#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////


	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ�*/
	virtual void StartConditionOnActor(AActor* Owner) {};

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime);

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ���� �Լ� */
	virtual void EndConditionOnActor(AActor * Owner);

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) {}

protected:
	/* UI Texture Opacity �ʱ�ȭ */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity �� �̿��� �����Ÿ� */
	void UpdateUIColorAndOpacity(UCBaseConditionType* ConditionData);

	/* Member */
public:
	EHitUpset GetState() const { return State; }
	void SetState(EHitUpset Type) { State = Type; }

protected:
	// @Target Info UI �� �������� ������ �ð�
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI �� �������� ����
	float OpacityLinearSpeed = 0.01f;
};
