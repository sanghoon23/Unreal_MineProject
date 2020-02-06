#include "CN_ItemDetach.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"

#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Hand.h"

void UCN_ItemDetach::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// Sword Mesh Attach 
	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MeshComp->GetOwner());
	IfNullRet(Charactor);

	ACDisplayItem* DisplayItem = Charactor->GetIEquipComp()->GetCurrentWeaponDisplay();
	IfNullRet(DisplayItem);
	check(DisplayItem);
	DisplayItem->MeshDetach();
}

