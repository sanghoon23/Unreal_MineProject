#include "CN_DeathCheck.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

void UCN_DeathCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(I_Charactor);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Delegatget ȣ�� */
	//@CharactorDestroy - ���°� ������ ������ ����� ��
	if (I_Charactor->IsDeath() == true)
	{
		//@Montage �� ���� ���̸� ���߰�, - Pause
		bool bMontagePlaying = MeshComp->GetAnimInstance()->Montage_IsPlaying(I_Charactor->GetCurrentApplyedMontage());
		if(bMontagePlaying == true)
		{
			MeshComp->GetAnimInstance()->Montage_Pause(I_Charactor->GetCurrentApplyedMontage());
		}

		//@OnCharactorDestory ����
		I_Charactor->OnCharactorDestroy.Broadcast();
	}
}
