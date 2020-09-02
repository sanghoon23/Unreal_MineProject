#include "CNS_HitMove.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCNS_HitMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	Direction = I_Charactor->GetIHitComp()->GetHitDirection();
	Speed = I_Charactor->GetIHitComp()->GetHitMoveSpeed();
}

// #Edit *0218 / 23:03
// Charactor Movement 사용, Input Value = 1.0f 로 고정 
// @Warning - OrientRotation 설정, 가속 되는 방향으로 회전을 꺼야됨
// 왜냐하면, 맞은 방향은 보고 있는 방향 반대이기 때문에 캐릭터가 돌아감.
void UCNS_HitMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	ACharacter* MovedCharactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(MovedCharactor);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	Direction = I_Charactor->GetIHitComp()->GetHitDirection();
	Speed = I_Charactor->GetIHitComp()->GetHitMoveSpeed();

	// @OrientRotation(false) - 가속 되는 방향으로 회전을 꺼야됨
	MovedCharactor->GetCharacterMovement()->bOrientRotationToMovement = false;

	// @Move - Default - Input Value = 1.0f 속도를 사용.
	MovedCharactor->AddMovementInput(Direction, Speed, true);
}

void UCNS_HitMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

	ACharacter* MovedCharactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(MovedCharactor);

	// @OrientRotation - 되돌리기
	MovedCharactor->GetCharacterMovement()->bOrientRotationToMovement = true;
}
