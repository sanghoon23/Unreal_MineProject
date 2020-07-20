#include "CNS_AttackMove.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCNS_AttackMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

// #Edit *0218 / 22:25
// @Charactor Movement 를 쓰는게 더 자연스러워보인다.
// 또한, 기본 움직이는 속도로 1.0f 으로 고정되게하니까
// 기준이 잡혀 수정하기 용이할듯 하다.
// @Warning - I_BaseAttack 의 Speed 는 없다.
void UCNS_AttackMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRet(I_AttackComp);

	IIC_BaseAttack* I_BaseAttack = I_AttackComp->GetCurrentIBaseAttack();
	IfNullRet(I_BaseAttack);

	ACharacter* MovedCharactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(MovedCharactor);

	// @Charactor Movement
	{
		FVector Dir = I_BaseAttack->GetAttackMoveDir();
		float Speed = I_BaseAttack->GetAttackMoveSpeed();

		// @기본적으로 Input Value = 1.0f 속도를 사용.
		MovedCharactor->AddMovementInput(Dir, Speed);
	}
	//// @Actor Location
	//{
	//	FVector Location = MovedCharactor->GetActorLocation();
	//	FVector Dir = I_BaseAttack->GetAttackMoveDir();
	//	float Speed = I_BaseAttack->GetAttackMoveSpeed();
	//	Location += (Dir * 10.0f);
	//	MovedCharactor->SetActorLocation(Location);
	//}
}

void UCNS_AttackMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

