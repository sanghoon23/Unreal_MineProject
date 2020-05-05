#include "CHM_BasicHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Basic.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "DamageType/CDamageType_Stun.h"
#include "DamageType/CDamageType_Burn.h"
#include "DamageType/CDamageType_Poision.h"
#include "DamageType/CDamageType_Freeze.h"

#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Stun.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Burn.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Poision.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Freeze.h"

UCHM_BasicHitComp::UCHM_BasicHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";
	#pragma region Hit Montages
	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_NormalHit1.HM_Basic_NormalHit1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'Air' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirHitFirst.HM_Basic_AirHitFirst'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHitMontage = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirAttackHit.HM_Basic_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHitMontage = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_StrongAttack.HM_Basic_StrongAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

	// 'Stun' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_Stun_.HM_Basic_Stun_'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> stunHit(*Path);
		if (stunHit.Succeeded())
			StunHitMontage = stunHit.Object;
	}

	#pragma endregion

	//@Load Poision Material
	{
		Path = L"Material'/Game/_Mine/Mesh/HM_Basic/CharM_Standard/M_Char_Standard_Poision.M_Char_Standard_Poision'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> PoisionMat(*Path);
		if (PoisionMat.Succeeded())
		{
			PoisionMaterial = PoisionMat.Object;
		}
	}

	//@Load Burn Particle - ParticleComp
	{
		BurnParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("BurnParticleComp");

		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_MG_BurnParticle.P_MG_BurnParticle'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> BurnPT(*Path);
		if (BurnPT.Succeeded())
		{
			BurnParticleComp->bAutoActivate = false;
			BurnParticleComp->SetTemplate(BurnPT.Object);
		}
	}
}

void UCHM_BasicHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Basic = Cast<ACHM_Basic>(GetOwner());
	AnimInst_HM_Basic = HM_Basic->GetMesh()->GetAnimInstance();

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		HM_Basic->OnGravity(); //@중력키기
	});
}


void UCHM_BasicHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_BasicHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	// @Interface
	IIC_Charactor* ICharactor = Cast<IIC_Charactor>(HM_Basic);
	check(ICharactor);

	//1. Delegate 실행.
	// @ResetState
	HM_Basic->OnActionResetState.Broadcast(HM_Basic);

	// @NORMAL - 일반 공격
	if (Type->GetConditionType() == FDamageType::NORMAL)
	{
		// @OnDamageDelegate
		// Type->OnDamageDelegate(Owner);

		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage


		//@Test Code
		//@다른 몽타주가 실행되고 있는지
		//bool IsOtherMonPlaying = AnimInst_HM_Basic->IsAnyMontagePlaying();
		//if (IsOtherMonPlaying == false)
		//{
		//}

		//1.4 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(NormalHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - NORMAL");
	}
	
	// @AIR - 띄우기
	else if (Type->GetConditionType() == FDamageType::AIR)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage

		//1.4 Jump
		ACharacter* Charactor = Cast<ACharacter>(HM_Basic);
		check(Charactor);
		FVector Location = Charactor->GetActorLocation();
		Location.Z += 200.0f;
		Charactor->SetActorLocation(Location);

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		HM_Basic->OffGravity();

		//1.6 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(AirHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - AIR");
	}

	// @AIR ATTACK
	else if (Type->GetConditionType() == FDamageType::AIRATTACK)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		HM_Basic->OffGravity();

		//1.5 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(AirAttackHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - AIRATTACK");
	}

	// @STRONG ATTACK
	else if (Type->GetConditionType() == FDamageType::STRONGATTACK)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//@애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(StrongAttackHitMontage, 0.6f, true);
	}

	// @STUN ATTACK
	else if (Type->GetConditionType() == FDamageType::STUN)
	{
		//@때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//@StunType 캐스팅
		UCDamageType_Stun* StunType = Cast<UCDamageType_Stun>(Type);
		check(StunType);

		//@상태이상 추가 - BaseHitComp
		UCUpset_Stun* UpsetStun = NewObject<UCUpset_Stun>();
		UpsetStun->ApplyTime = StunType->GetStunTime();
		UpsetStun->SetMontage(StunHitMontage);

		UTexture2D* Texture = StunType->GetUITexture();
		if (Texture != nullptr)
		{
			UpsetStun->TextureUI = Texture;
		}

		//@애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(StunHitMontage, 0.6f, true);

		bool bAddResult = AddConditionData(UpsetStun);
		if (bAddResult == false)
		{
			UE_LOG(LogTemp, Warning, L"HM_BasicHitComp STUN AddConditionData Derived NULL!!");
		}
	}

	// @BURN ATTACK
	else if (Type->GetConditionType() == FDamageType::BURN)
	{
		//@BurnType 캐스팅
		UCDamageType_Burn* BurnType = Cast<UCDamageType_Burn>(Type);
		check(BurnType);

		UCUpset_Burn* BurnConditionData = NewObject<UCUpset_Burn>();
		check(BurnConditionData);
		BurnConditionData->ApplyTime = BurnType->GetBurnTime();
		BurnConditionData->SetBurnParticleComp(BurnParticleComp);

		UTexture2D* Texture = BurnType->GetUITexture();
		if (Texture != nullptr)
		{
			BurnConditionData->TextureUI = Texture;
		}

		bool bAddResult = AddConditionData(BurnConditionData);
		if (bAddResult == false)
		{
			UE_LOG(LogTemp, Warning, L"HM_BasicHitComp BURN AddConditionData Derived NULL!!");
		}
	}

	// @POISION ATTACK
	else if (Type->GetConditionType() == FDamageType::POISION)
	{
		//@PoisionType 캐스팅
		UCDamageType_Poision* PoisionType = Cast<UCDamageType_Poision>(Type);
		check(PoisionType);

		//UCUpset_Poision* PoisionConditionData = NewObject<UCUpset_Poision>();
		UCUpset_Poision* PoisionConditionData = NewObject<UCUpset_Poision>();
		check(PoisionConditionData);
		PoisionConditionData->ApplyTime = PoisionType->GetPoisioningTime();
		PoisionConditionData->SetOriginMaterial(HM_Basic->GetMesh()->GetMaterial(0));

		UTexture2D* Texture = PoisionType->GetUITexture();
		if (Texture != nullptr)
		{
			PoisionConditionData->TextureUI = Texture;
		}

		//@Poision Material 변경
		HM_Basic->GetMesh()->SetMaterial(0, PoisionMaterial);

		bool bAddResult = AddConditionData(PoisionConditionData);
		if (bAddResult == false)
		{
			UE_LOG(LogTemp, Warning, L"HM_BasicHitComp POISION AddConditionData Derived NULL!!");
		}
	}

	// @FREEZE ATTACK
	else if (Type->GetConditionType() == FDamageType::FREEZE)
	{
		//@PoisionType 캐스팅
		UCDamageType_Freeze* FreezeType = Cast<UCDamageType_Freeze>(Type);
		check(FreezeType);

		//UCUpset_Poision* PoisionConditionData = NewObject<UCUpset_Poision>();
		UCUpset_Freeze* FreezeConditionData = NewObject<UCUpset_Freeze>();
		check(FreezeConditionData);
		FreezeConditionData->ApplyTime = FreezeType->GetFreezingTime();

		UTexture2D* Texture = FreezeType->GetUITexture();
		if (Texture != nullptr)
		{
			FreezeConditionData->TextureUI = Texture;
		}

		bool bAddResult = AddConditionData(FreezeConditionData);
		if (bAddResult == false)
		{
			UE_LOG(LogTemp, Warning, L"HM_BasicHitComp FREEZE AddConditionData Derived NULL!!");
		}
	}

}
