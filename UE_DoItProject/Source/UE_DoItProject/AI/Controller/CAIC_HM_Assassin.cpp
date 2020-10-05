#include "CAIC_HM_Assassin.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "Charactor/Monster/CHM_Assassin.h"

ACAIC_HM_Assassin::ACAIC_HM_Assassin()
{
	FString path = L"";

	PatrolRadius = 400.0f;
	DetectRadius = 5000.0f;
	AttackRange = 500.0f;

	path = L"BlackboardData'/Game/_Mine/_MyBlueprint/AI/Blackboard/BB_HM_Assassin.BB_HM_Assassin'";
	ConstructorHelpers::FObjectFinder<UBlackboardData> bb(*path);
	if (bb.Succeeded())
		BB = bb.Object;

	path = L"BehaviorTree'/Game/_Mine/_MyBlueprint/AI/BehaviorTree/BT_HM_Assassin.BT_HM_Assassin'";
	ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(*path);
	if (bt.Succeeded())
		BT = bt.Object;
}

void ACAIC_HM_Assassin::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsVector("Destination", FVector(0.0f));
}

void ACAIC_HM_Assassin::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	// AI Controller �� �������� ������,
	IfNullRet(InPawn->AIControllerClass);

	//@Insert Pattern Attack - (���� ���� ���� ����)
	{
		//TODO : SharedPT �Ẹ��.
		/* �����ϴ�.
		������, ������ �⺻������ AttackComp �� SetTypeBaseAttack �� ����ؼ�
		���°��� ����� ���ÿ� ��Ÿ���� �����͵��� �����̰� �־,
		Shared �� �ع����ԵǸ� ���۵��� ��� ����������. 
		�׸��� ������ ������ ���� �����鼭 BeginAttack �� ��� �����ϱ� ������
		��� ���� ������ ��
		--> �̿� ���� �ذ�å, �׳� Type �� ��������. - Index Ȱ��
		�ش� ĳ������ ����Ÿ���� �����ϰ� ��ü���� ����.
		*/
		//TSharedPtr<UC_BaseAttackState> SharedPT_AttackOne;
		//MakeShared<FMyObjectType, ESPMode::ThreadSafe>
		//TSharedPtr<UC_BaseAttackState> SharedPT_AttackOne;
		//SharedPT_AttackOne = MakeShareable<UC_BaseAttackState>(Attack_One);
		//PatternAttackList.Add(SharedPT_AttackOne);
		//uint8 Two = static_cast<uint8>(EHM_AssassinAttackType::SECONDATTACK);
		//UC_BaseAttackState* Attack_Two = IC_AttackComp->GetBaseAttack(Two);
		//check(Attack_Two);
		//TSharedPtr<UC_BaseAttackState> SharedPT_AttackTwo;
		//SharedPT_AttackTwo = MakeShareable<UC_BaseAttackState>(Attack_Two);
		//PatternAttackTypeList.Add(SharedPT_AttackTwo);

		int FirstAttack = static_cast<int>(EHM_AssassinAttackType::FIRSTATTACK);
		PatternAttackTypeList.Add(FirstAttack);

		int SecondAttack = static_cast<int>(EHM_AssassinAttackType::SECONDATTACK);
		PatternAttackTypeList.Add(SecondAttack);

		int FourAttack = static_cast<int>(EHM_AssassinAttackType::FOURATTACK);
		PatternAttackTypeList.Add(FourAttack);

		int SixAttack = static_cast<int>(EHM_AssassinAttackType::SIXATTACK);
		PatternAttackTypeList.Add(SixAttack);
	}

	//@RandomAttack - 
	{
		////@ǥâ ������
		//int ThirdAttack = static_cast<int>(EHM_AssassinAttackType::THIRDATTACK);
		//RandomAttackTypeList.Add(ThirdAttack);

		//@ī�޶� ��ȯ ���� -> �г� ���� �� ����ְ� ����.
		//int FourAttack = static_cast<int>(EHM_AssassinAttackType::FOURATTACK);
		//RandomAttackTypeList.Add(FourAttack);
	}

	if (UseBlackboard(BB, Blackboard))
	{
		Blackboard->SetValueAsVector("Home", InPawn->GetActorLocation());
		Blackboard->SetValueAsFloat("PatrolRadius", PatrolRadius);
		Blackboard->SetValueAsFloat("DetectRadius", DetectRadius);
		Blackboard->SetValueAsFloat("AttackRange", AttackRange);

		RunBehaviorTree(BT);
	}
}

void ACAIC_HM_Assassin::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIC_HM_Assassin::AddTypePatternAttack(EHM_AssassinAttackType Type)
{
	int TypeNum = static_cast<int>(Type);
	PatternAttackTypeList.Add(TypeNum);
}

void ACAIC_HM_Assassin::AddTypeRandomAttack(EHM_AssassinAttackType Type)
{
	int TypeNum = static_cast<int>(Type);
	RandomAttackTypeList.Add(TypeNum);
}

