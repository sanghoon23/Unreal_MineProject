#include "CDamageType_Stun.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Component/Base/C_BaseHitComp.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Stun.h"

UCDamageType_Stun::UCDamageType_Stun()
{
	// Super
	{
		TypeNumber = 4;
		DamageType = FDamageType::STUN;
	}

	// Attribute
	{
		bCausedByWorld = false;
		bScaleMomentumByMass = true;
		bRadialDamageVelChange = false;

		DamageImpulse = 100.0f;
		DestructibleImpulse = 100.0f;
		DestructibleDamageSpreadScale = 100.0f;
	}

	FString strPath = L"";

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_StunTypeFilling.Tex_StunTypeFilling'";
	ConstructorHelpers::FObjectFinder<UTexture2D> StunTexture(*strPath);
	if (StunTexture.Succeeded())
	{
		StunConditionUITexture = StunTexture.Object;
	}

	strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_StunActor.PS_StunActor'";
	ConstructorHelpers::FObjectFinder<UParticleSystem> P_StunHead(*strPath);
	if (P_StunHead.Succeeded())
	{
		StunHeadParticle = P_StunHead.Object;
	}
}

void UCDamageType_Stun::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@����ó��
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	//@���� ��� �ٶ󺸱�
	//UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	APawn* const DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);

	AController* const PawnController = Cast<APawn>(Subject)->GetController();
	check(PawnController);

	IIC_Charactor* const I_Charactor = Cast<IIC_Charactor>(DamagedPawn);
	check(I_Charactor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Set UpsetStun Setting
	UCUpset_Stun* UpsetStun = NewObject<UCUpset_Stun>();
	check(UpsetStun);
	UpsetStun->SetCauser(Subject); //@��ü�� ����
	UpsetStun->ApplyTime = GetStunTime();
	UpsetStun->SetDamageSubjectController(PawnController);

	const uint8 StunMontageNum = static_cast<uint8>(FDamageType::STUN);
	UAnimMontage* StunMontage = DamagedActorHitComp->GetDamagedMontageOrNull(StunMontageNum);
	if (StunMontage != nullptr)
	{
		UpsetStun->SetMontage(DamagedActorHitComp->GetDamagedMontageOrNull(StunMontageNum));
	}

	UParticleSystem* HitCompStunHeadParticle = DamagedActorHitComp->GetStunHeadParticleOrNull();
	if (HitCompStunHeadParticle != nullptr)
	{
		UpsetStun->SetStunHeadPrticle(HitCompStunHeadParticle);
	}
	else //@Default
	{
		UpsetStun->SetStunHeadPrticle(StunHeadParticle);
	}

	//@Take Damage
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	UpsetStun->SetDamageEvent(DamageEvent); //@Set
	//#1112_�������� ���� ����.
	//������ �׾��� ��, �� ��Ÿ�ְ� ����Ǿ����.
	//TakeDamage ����, IsDeath() �� DamagedActor �� �׾����� �Ǻ�.
	/*
	TakeDamage �� BaseHitComp �� ���� �ʰ� ���⿡ ���� �� �ۿ� ������ ������
	�� DamageEvent �� �ش� DamageType ��ü �ȿ��� ����ְ� �;��� ����.
	BaseHitComp �� �ϰ� �Ǹ� BaseDamageType* ���� ������ ������
	�� ��ü�� ��� ��ü���� �к��ϱ� ���� if �� ���� ����. (if �� ���̱� ���ؼ�)
	���� TakeDamage �� ĳ���� �� ��ü�� �� ��
	�������� - PawnController
	��� ���� - �ش� DamageType �� Class. ���� ���� ����.
	���� TakeDamage ȣ�� ��, IsDeath() �� ĳ���Ͱ� �׾����� Ȯ�����־�� �Ѵٴ� ���� ����.
	*/
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, Subject);
	IfTrueRet(I_Charactor->IsDeath()); //@ĳ���Ͱ� �׾��ٸ�,

	//@DamageTypeEffet �� ������� �ʴ´ٸ�, Damage ��, ����.
	const uint8 MontageTypeNum = static_cast<uint8>(GetConditionType());
	IfFalseRet(DamagedActorHitComp->IsUsingDamageTypeEffect(MontageTypeNum));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UTexture2D* Texture = GetUITexture();
	if (Texture != nullptr)
	{
		UpsetStun->SetTextureUI(Texture);
	}

	//@Copy Delegate
	{
		UpsetStun->OnDelStartCondition = OnLinkStartUpsetCondition;
		UpsetStun->OnDelEndCondition = OnLinkEndUpsetCondition;
	}

	bool bAddResult = DamagedActorHitComp->AddConditionData(UpsetStun);
	if (bAddResult == false)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp STUN AddConditionData Derived NULL!!");
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Montage
	{
		//@Montage ���� �� �� ���� �������� Ȯ��.
		IfTrueRet(I_Charactor->IsDontMontagePlay());

		const uint8 MontageNum = static_cast<uint8>(GetConditionType());
		DamagedActorHitComp->RunMontageFromAttackType(EComboOrNot::NONE, MontageNum, 0.6f, true);
	}

}

void UCDamageType_Stun::SettingData(const FDamageData & Data)
{
	DamageImpulse = Data.DamageImpulse;
	SetStunTime(Data.StunTime);
}
