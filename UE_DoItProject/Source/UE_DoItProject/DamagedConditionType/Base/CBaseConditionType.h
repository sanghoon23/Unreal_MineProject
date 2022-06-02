#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


#include "CBaseConditionType.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelStartCondition, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FDelEndCondition, AActor*)

UENUM()
/* ���� �̻� ���� */
enum class EHitUpset : uint8
{
	NONE		= 0,
	SLOWER		= 1, //�̵�����
	STUN		= 2, //����
	BURN		= 3, //ȭ��
	POISION		= 4, //�ߵ�
	FREEZE		= 5, //����
	SLEEP		= 6, //����
};

UCLASS()
class UE_DOITPROJECT_API UCBaseConditionType 
	: public UObject
{
	GENERATED_BODY()

public:
	/* HitComp ���� Condition ��Ȳ�� '����' �Ǿ��� ��, ����� Delegate */
	FDelStartCondition OnDelStartCondition;

	/* HitComp ���� Condition ��Ȳ�� '����' �Ǿ��� ��, ����� Delegate */
	FDelEndCondition OnDelEndCondition;

	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� - TintColor �� UI ������ ȿ�� / ������ ȿ�� ���� */
		FLinearColor TintColor = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* ���� ���� �ð� */
		float ApplyTime = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo ǥ�� Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* @true - LeftToRight, @false - RightToLeft*/
		bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		EHitUpset State;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		FDamageEvent DamageEvent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		class AController* DamageSubjectController;

	#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

public:
	UCBaseConditionType();

	/* @Warning - Texture �� �������� ���� */
	void Copy(const UCBaseConditionType* const In);


	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ� */
	virtual void StartCondition(APawn* Owner);

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime);

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ���� �Լ� */
	virtual void EndCondition(APawn* Owner);

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Function */
public:
	class UTexture2D* GetTextureUI() { return TextureUI; }
	void SetTextureUI(class UTexture2D* Texture);

	/* ConditionType Damage �� ����Ų ��ü�� ���� */
	void SetCauser(AActor* InCauser) { Causer = InCauser; }

protected:
	/* UI Texture Opacity �ʱ�ȭ */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity �� �̿��� �����Ÿ� */
	void UpdateUIColorAndOpacity(UCBaseConditionType* ConditionData);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Member */
public:
	EHitUpset GetState() const { return State; }
	void SetState(EHitUpset Type) { State = Type; }

	const FDamageEvent& GetDamageEvent() const { return DamageEvent; }
	void SetDamageEvent(const FDamageEvent& Event) { DamageEvent = Event; }

	class AController* GetDamageSubjectController() { return DamageSubjectController; }
	void SetDamageSubjectController(class AController* InputController);

	float GetApplyTime() const { return ApplyTime; }

protected:
	// @Target Info UI �� �������� ������ �ð�
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI �� �������� ����
	float OpacityLinearSpeed = 0.01f;

	//@Damage �� �� Causer (��ü��)
	AActor* Causer;
};
