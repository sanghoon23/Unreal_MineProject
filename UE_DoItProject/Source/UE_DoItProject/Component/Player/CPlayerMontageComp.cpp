#include "CPlayerMontageComp.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_MontageComp.h"
#include "Charactor/Player/CPlayer.h"

UCPlayerMontageComp::UCPlayerMontageComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// ...
#pragma region Common Montage

#pragma endregion

#pragma region Move Montage

	UAnimMontage* Evade_F;
	UAnimMontage* Evade_B;
	UAnimMontage* Evade_L;
	UAnimMontage* Evade_R;

	// Setting
	{
		// Evade_F
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/SK_Mannequin_Mage_Evade_F_Montage.SK_Mannequin_Mage_Evade_F_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MEvade_F(*Path);
		if (MEvade_F.Succeeded())
			Evade_F = MEvade_F.Object;
		MoveMontages.Add(Evade_F);

		// Evade_B
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/SK_Mannequin_Mage_Evade_B_Montage.SK_Mannequin_Mage_Evade_B_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MEvade_B(*Path);
		if (MEvade_B.Succeeded())
			Evade_B = MEvade_B.Object;
		MoveMontages.Add(Evade_B);

		// Evade_L
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/SK_Mannequin_Mage_Evade_L_Montage.SK_Mannequin_Mage_Evade_L_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MEvade_L(*Path);
		if (MEvade_L.Succeeded())
			Evade_L = MEvade_L.Object;
		MoveMontages.Add(Evade_L);

		// Evade_R
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/SK_Mannequin_Mage_Evade_R_Montage.SK_Mannequin_Mage_Evade_R_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MEvade_R(*Path);
		if (MEvade_R.Succeeded())
			Evade_R = MEvade_R.Object;
		MoveMontages.Add(Evade_R);
	}

#pragma endregion

#pragma region Mage Montage

#pragma endregion

#pragma region Sword Montage

#pragma endregion
}


void UCPlayerMontageComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPlayerMontageComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/////////////////////////////////////////////////////////////////////////////////
// @MontageType - ��Ÿ�� Ÿ��(Ex.UMageMontageType, UCommonMontageType)
// @PlayAnimNum - Type �� �ش��ϴ� �ִϸ��̼� ����.
// @Sort - �ش� Actor�� ��ü ��Ÿ�� ����(MontageSort)(Ex. Move, Sword, Mage...)
// @Speed - �ִϸ��̼�(��Ÿ��) �ӵ�
// @bAlways - �ִϸ��̼�(��Ÿ��) �� ���༺ ����
// True �� ������ ����, False�� �ִϸ��̼�(��Ÿ��) ���� �� ���۾���.
// @���� - ���⼭ Common, Move�� Map �� ����Ǿ������ʰ�, Array�� �����ϰ� �ִ�.
// �׷��� ���� Common�� Move ���� Monatge�� ����Ϸ��ϸ�, Type = 0 ���� �ָ� �ȴ�.
void UCPlayerMontageComp::PlayAnimation(UINT MontageType, UINT PlayAnimNum, UINT Sort, float Speed, bool bAlways)
{
	// * Common - (MontageType = 0)
	if (static_cast<MontageSort>(Sort) == MontageSort::Common)
	{
		// PlayAnimNum
		IIC_Charactor* OwnerActor = Cast<IIC_Charactor>(GetOwner());
		check(OwnerActor);
		OwnerActor->ActorAnimMonPlay(CommonMontages[PlayAnimNum], Speed, bAlways);
	}
	// * Move - (MontageType = 0)
	else if (static_cast<MontageSort>(Sort) == MontageSort::Move)
	{
		// PlayAnimNum
		IIC_Charactor* OwnerActor = Cast<IIC_Charactor>(GetOwner());
		check(OwnerActor);
		OwnerActor->ActorAnimMonPlay(MoveMontages[PlayAnimNum], Speed, bAlways);
	}
	// * Mage
	else if (static_cast<MontageSort>(Sort) == MontageSort::Mage)
	{
		UMageMontageType Type = static_cast<UMageMontageType>(MontageType);

		TArray<UAnimMontage*>* MontageList = MageMontageMap.Find(Type);
		check(MontageList);
		if (MontageList != nullptr)
		{
			// UINT AnimNum = static_cast<UINT>(PlayAnim);
			IIC_Charactor* OwnerActor = Cast<IIC_Charactor>(GetOwner());
			check(OwnerActor);

			OwnerActor->ActorAnimMonPlay((*MontageList)[PlayAnimNum], Speed, bAlways);
		}
	}//(Mage)
	// *Sword
	else if (static_cast<MontageSort>(Sort) == MontageSort::Sword)
	{
		USwordMontageType Type = static_cast<USwordMontageType>(MontageType);

		TArray<UAnimMontage*>* MontageList = SwordMontageMap.Find(Type);
		check(MontageList);
		if (MontageList != nullptr)
		{
			// UINT AnimNum = static_cast<UINT>(PlayAnim);
			IIC_Charactor* OwnerActor = Cast<IIC_Charactor>(GetOwner());
			check(OwnerActor);

			OwnerActor->ActorAnimMonPlay((*MontageList)[PlayAnimNum], Speed, bAlways);
		}
	}//(Sword)

}

// - Player Montage ������ ���� Map �� Montage ADD �Լ�
// Ex) Attack �� ������ Mage, Sword
// @MontageType - �ش� Montage Ÿ��
// @Sort - �ش� Montage ����(MontageSort)
void UCPlayerMontageComp::MontageAddInMap(UINT MontageType, UINT Sort, UAnimMontage* Montage)
{
	check(Montage);
	IfNullRet(Montage);

	// * Insert MageMap
	if (static_cast<MontageSort>(Sort) == MontageSort::Mage)
	{
		UMageMontageType Type = static_cast<UMageMontageType>(MontageType);
		TArray<UAnimMontage*>* ArrayValue = MageMontageMap.Find(Type);
		if (ArrayValue == nullptr) // TArray ����.
		{
			TArray<UAnimMontage*> insertArrayValue;
			insertArrayValue.Add(Montage);
			MageMontageMap.Add(Type, insertArrayValue);
		}
		else // TArray �� ����.
		{
			ArrayValue->Add(Montage);
		}

	}//(Mage)
	// * Insert SwordMap
	else if (static_cast<MontageSort>(Sort) == MontageSort::Sword)
	{
		USwordMontageType Type = static_cast<USwordMontageType>(MontageType);
		TArray<UAnimMontage*>* ArrayValue = SwordMontageMap.Find(Type);
		if (ArrayValue == nullptr) // TArray ����.
		{
			TArray<UAnimMontage*> insertArrayValue;
			insertArrayValue.Add(Montage);
			SwordMontageMap.Add(Type, insertArrayValue);
		}
		else // TArray �� ����.
		{
			ArrayValue->Add(Montage);
		}

	}//(Sword)
}

