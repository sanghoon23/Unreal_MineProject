#include "CNS_PlayerBlockActionAndMove.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"

void UCNS_PlayerBlockActionAndMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	//1.
	//ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	//check(Charactor);
	//APlayerController* PlayerController = Cast<APlayerController>(Charactor->GetController());
	//if (PlayerController != nullptr)
	//{
	//	Charactor->DisableInput(PlayerController);
	//}

	//2. INPUT BLOCK ON
	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
	IfNullRet(I_Charactor);
	I_Charactor->CanNotMove();
	IIC_Player* I_Player = Cast<IIC_Player>(Charactor);
	IfNullRet(I_Player);
	I_Player->OnBlockAction();
}

void UCNS_PlayerBlockActionAndMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_PlayerBlockActionAndMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	//1.
	//ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	//check(Charactor);
	//APlayerController* PlayerController = Cast<APlayerController>(Charactor->GetController());
	//if (PlayerController != nullptr)
	//{
	//	Charactor->EnableInput(PlayerController);
	//}

	// 2. INPUT BLOCK OFF
	ACharacter* Charactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
	IfNullRet(I_Charactor);
	I_Charactor->CanMove();
	IIC_Player* I_Player = Cast<IIC_Player>(Charactor);
	IfNullRet(I_Player);
	I_Player->OffBlockAction();

}
