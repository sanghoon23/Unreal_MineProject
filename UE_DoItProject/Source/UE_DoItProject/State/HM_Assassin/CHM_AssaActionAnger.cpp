#include "CHM_AssaActionAnger.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface//IC_HitComp.h"
#include "Interface//IC_MeshParticle.h"
#include "Charactor/Monster/CHM_Assassin.h"

#include "Ability/Monster/CHMAbility_ATKUpper.h"


UCHM_AssaActionAnger::UCHM_AssaActionAnger()
{
	PrimaryComponentTick.bCanEverTick = true;
	//..

	FString Path = L"";

	// Set Particle
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/P_Assa_Ability_Angry.P_Assa_Ability_Angry'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_Load(*Path);
		if (P_Load.Succeeded())
			P_Action = P_Load.Object;

		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Assassin/P_Assa_Ability_Angry_OutsideEffect.P_Assa_Ability_Angry_OutsideEffect'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_LoadSideEft(*Path);
		if (P_LoadSideEft.Succeeded())
			P_Action_SideEffect = P_LoadSideEft.Object;
	}

#pragma region Set Montage
	// Set Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Motion/HM_Assassin_Mon_PazStart.HM_Assassin_Mon_PazStart'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> PlayMon(*Path);
		if (PlayMon.Succeeded())
			ActionMontage = PlayMon.Object;
	}

#pragma endregion

}

void UCHM_AssaActionAnger::BeginPlay()
{
	Super::BeginPlay();

	//@Get Player Pointer
	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);
}

void UCHM_AssaActionAnger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssaActionAnger::OnAction()
{
	Super::OnAction();

	//IIC_BaseAttack* BaseAttack = HM_Assassin->GetIAttackComp()->GetCurrentIBaseAttack();
	//if (BaseAttack != nullptr)
	//{
	//	IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	//}

	//TODO : 공중에 띄워진 상태에서 HP 가 반으로 준다면??-> 일단 보류

	// @IF TRUE RETURN
	IfTrueRet(HM_Assassin->GetCharacterMovement()->IsFalling()); //@IsFalling

	//// @IF FALSE RETURN
	//IfFalseRet(Player->GetCanMove());

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// @Reset
	HM_Assassin->OnActionResetState.Broadcast(HM_Assassin);

	// @SetCurrentBaseAction
	HM_Assassin->SetCurrentBaseAction(this);

	// @몽타주 실행
	HM_Assassin->ActorAnimMonPlay(ActionMontage, 1.0f, true);
}

void UCHM_AssaActionAnger::BeginActionState()
{
	Super::BeginActionState();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Dont Montage
	HM_Assassin->SetDontMontagePlay(true);

	//@AI OFF
	IIC_Monster* I_Monster = Cast<IIC_Monster>(HM_Assassin);
	check(I_Monster);
	I_Monster->SetAIRunningPossible(false);

	//@무적 ON
	IIC_HitComp* I_HitComp = HM_Assassin->GetIHitComp();
	check(I_HitComp);
	I_HitComp->SetCanAttackedFromOther(false);
	I_HitComp->SetDamagedFromOther(false);

	//@Particle 삽입
	IIC_MeshParticle* I_MeshParticle = HM_Assassin->GetIMeshParticle();
	if (I_MeshParticle != nullptr)
	{
		UParticleSystemComponent* AttachPTComp = I_MeshParticle->SpawnParticleAtMesh
		(
			P_Action,
			EAttachPointType::ROOT,
			EAttachPointRelative::RELATIVE,
			EAttachLocation::SnapToTarget
		);

		if (AttachPTComp == nullptr)
		{
			UE_LOG(LogTemp, Warning, L"CUpset_Burn BurnParticleComp NULL!!");
		}
		else
		{
			FTransform Transform;
			Transform.SetRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
			AttachPTComp->SetRelativeTransform(Transform);

			HM_Assassin->OnDeathDelegate.AddLambda([AttachPTComp]()
			{
				AttachPTComp->SetActive(false);
			});
		}
	}
}

void UCHM_AssaActionAnger::TickActionState()
{
	Super::TickActionState();
}

void UCHM_AssaActionAnger::EndActionState()
{
	Super::EndActionState();

	//@Do Montage
	HM_Assassin->SetDontMontagePlay(false);

	//@AI ON
	IIC_Monster* I_Monster = Cast<IIC_Monster>(HM_Assassin);
	check(I_Monster);
	I_Monster->SetAIRunningPossible(true);

	//@무적 OFF
	IIC_HitComp* I_HitComp = HM_Assassin->GetIHitComp();
	check(I_HitComp);
	I_HitComp->SetCanAttackedFromOther(true);
	I_HitComp->SetDamagedFromOther(true);

	//@Ability 삽입
	IIC_AbilityComp* I_AbilityComp = HM_Assassin->GetIAbilityComp();
	if (I_AbilityComp != nullptr)
	{
		//@ADD Ability
		UCHMAbility_ATKUpper* Ability_ATKUpper = NewObject<UCHMAbility_ATKUpper>();

		//@Create Ability
		FAbilityValue InputValue;
		InputValue.Sort = EAbilitySort::SAVEARRAY;
		InputValue.bTimer = false;
		InputValue.Timer = 0.0f;
		InputValue.Value = 0.0f;
		Ability_ATKUpper->SetAbilityValue(InputValue);
		Ability_ATKUpper->SetUsingColorOpacity(false);
		Ability_ATKUpper->SetAppliedActor(HM_Assassin);
		Ability_ATKUpper->SetATKUpperCoefficient(0.5f);

		I_AbilityComp->AddAbility(Ability_ATKUpper);
	}

	//@Particle 삽입
	IIC_MeshParticle* I_MeshParticle = HM_Assassin->GetIMeshParticle();
	if (I_MeshParticle != nullptr)
	{
		UParticleSystemComponent* AttachPTComp_SideEft = I_MeshParticle->SpawnParticleAtMesh
		(
			P_Action_SideEffect,
			EAttachPointType::ROOT,
			EAttachPointRelative::RELATIVE,
			EAttachLocation::SnapToTarget
		);

		if (AttachPTComp_SideEft == nullptr)
		{
			UE_LOG(LogTemp, Warning, L"CUpset_Burn BurnParticleComp NULL!!");
		}
		else
		{
			FTransform Transform;
			Transform.SetScale3D(FVector(2.0f));
			AttachPTComp_SideEft->SetRelativeTransform(Transform);

			HM_Assassin->OnDeathDelegate.AddLambda([AttachPTComp_SideEft]()
			{
				AttachPTComp_SideEft->SetActive(false);
			});
		}
	}
}
