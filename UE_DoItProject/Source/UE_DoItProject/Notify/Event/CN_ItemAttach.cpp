#include "CN_ItemAttach.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"

#include "Item/Base/CItem_Hand.h"

void UCN_ItemAttach::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// Sword Mesh Attach 
	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	ACItem_Hand* HandItem = Charactor->GetIEquipComp()->GetCurrentWeaponDisplay();
	IfNullRet(HandItem);
	check(HandItem);
	HandItem->MeshAttach(HandItem->GetItemEquipName());
}

