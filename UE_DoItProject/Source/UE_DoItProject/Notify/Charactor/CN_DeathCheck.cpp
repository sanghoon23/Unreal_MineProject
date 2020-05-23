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

	/* Delegatget 호출 */
	//@CharactorDestroy - 상태가 완전히 끝나고 사라질 때
	if (I_Charactor->IsDeath() == true)
	{
		//@Montage 가 실행 중이면 멈추고, - Pause
		bool bMontagePlaying = MeshComp->GetAnimInstance()->Montage_IsPlaying(I_Charactor->GetCurrentApplyedMontage());
		if(bMontagePlaying == true)
		{
			MeshComp->GetAnimInstance()->Montage_Pause(I_Charactor->GetCurrentApplyedMontage());
		}

		//@OnCharactorDestory 실행
		I_Charactor->OnCharactorDestroy.Broadcast();
	}
}
