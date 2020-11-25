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

	//@예외처리
	IfFalseRet(DamagedActorHitComp->IsDamagedFromOther());

	//@때린 대상 바라보기
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
	UpsetStun->SetCauser(Subject); //@주체자 설정
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
	//#1112_데미지만 들어가게 가능.
	//하지만 죽었을 때, 그 몽타주가 실행되어버림.
	//TakeDamage 이후, IsDeath() 로 DamagedActor 가 죽었는지 판별.
	/*
	TakeDamage 를 BaseHitComp 에 넣지 않고 여기에 넣을 수 밖에 없었던 이유는
	위 DamageEvent 를 해당 DamageType 객체 안에서 집어넣고 싶었기 때문.
	BaseHitComp 에 하게 되면 BaseDamageType* 으로 들어오기 때문에
	각 객체가 어떠한 객체인지 분별하기 위해 if 를 많이 쓴다. (if 를 줄이기 위해서)
	따라서 TakeDamage 로 캐릭터 안 객체로 들어갈 때
	누구한테 - PawnController
	어떠한 공격 - 해당 DamageType 의 Class. 으로 구별 가능.
	단지 TakeDamage 호출 시, IsDeath() 로 캐릭터가 죽었는지 확인해주어야 한다는 단점 존재.
	*/
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, Subject);
	IfTrueRet(I_Charactor->IsDeath()); //@캐릭터가 죽었다면,

	//@DamageTypeEffet 를 사용하지 않는다면, Damage 만, 들어간다.
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
		//@Montage 실행 할 수 없는 상태인지 확인.
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
