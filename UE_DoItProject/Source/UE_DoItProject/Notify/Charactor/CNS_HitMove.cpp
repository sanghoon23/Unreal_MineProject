#include "CNS_HitMove.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

void UCNS_HitMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	Direction = Charactor->GetIHitComp()->GetHitDirection();
	Speed = Charactor->GetIHitComp()->GetHitMoveSpeed();
}

// #Edit *0218 / 23:03
// Charactor Movement ���, Input Value = 1.0f �� ���� 
// @Warning - OrientRotation ����, ���� �Ǵ� �������� ȸ���� ���ߵ�
// �ֳ��ϸ�, ���� ������ ���� �ִ� ���� �ݴ��̱� ������ ĳ���Ͱ� ���ư�.
void UCNS_HitMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	ACharacter* MovedCharactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(MovedCharactor);

	// @OrientRotation(false) - ���� �Ǵ� �������� ȸ���� ���ߵ�
	MovedCharactor->GetCharacterMovement()->bOrientRotationToMovement = false;

	// @Move - Default - Input Value = 1.0f �ӵ��� ���.
	MovedCharactor->AddMovementInput(Direction, Speed);
}

void UCNS_HitMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACharacter* MovedCharactor = Cast<ACharacter>(MeshComp->GetOwner());
	IfNullRet(MovedCharactor);

	// @OrientRotation - �ǵ�����
	MovedCharactor->GetCharacterMovement()->bOrientRotationToMovement = true;

}
