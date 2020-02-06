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
// @MontageType - 몽타주 타입(Ex.UMageMontageType, UCommonMontageType)
// @PlayAnimNum - Type 에 해당하는 애니메이션 순번.
// @Sort - 해당 Actor의 전체 몽타주 종류(MontageSort)(Ex. Move, Sword, Mage...)
// @Speed - 애니메이션(몽타주) 속도
// @bAlways - 애니메이션(몽타주) 의 실행성 보장
// True 면 무조건 실행, False면 애니메이션(몽타주) 실행 시 동작안함.
// @참고 - 여기서 Common, Move는 Map 에 저장되어있지않고, Array로 관리하고 있다.
// 그래서 만약 Common과 Move 관련 Monatge를 사용하려하면, Type = 0 으로 주면 된다.
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

// - Player Montage 관리를 위한 Map 에 Montage ADD 함수
// Ex) Attack 에 관련한 Mage, Sword
// @MontageType - 해당 Montage 타입
// @Sort - 해당 Montage 종류(MontageSort)
void UCPlayerMontageComp::MontageAddInMap(UINT MontageType, UINT Sort, UAnimMontage* Montage)
{
	check(Montage);
	IfNullRet(Montage);

	// * Insert MageMap
	if (static_cast<MontageSort>(Sort) == MontageSort::Mage)
	{
		UMageMontageType Type = static_cast<UMageMontageType>(MontageType);
		TArray<UAnimMontage*>* ArrayValue = MageMontageMap.Find(Type);
		if (ArrayValue == nullptr) // TArray 생성.
		{
			TArray<UAnimMontage*> insertArrayValue;
			insertArrayValue.Add(Montage);
			MageMontageMap.Add(Type, insertArrayValue);
		}
		else // TArray 에 삽입.
		{
			ArrayValue->Add(Montage);
		}

	}//(Mage)
	// * Insert SwordMap
	else if (static_cast<MontageSort>(Sort) == MontageSort::Sword)
	{
		USwordMontageType Type = static_cast<USwordMontageType>(MontageType);
		TArray<UAnimMontage*>* ArrayValue = SwordMontageMap.Find(Type);
		if (ArrayValue == nullptr) // TArray 생성.
		{
			TArray<UAnimMontage*> insertArrayValue;
			insertArrayValue.Add(Montage);
			SwordMontageMap.Add(Type, insertArrayValue);
		}
		else // TArray 에 삽입.
		{
			ArrayValue->Add(Montage);
		}

	}//(Sword)
}

