#include "CItem_Event.h"
#include "Global.h"

ACItem_Event::ACItem_Event()
{
	// Super
	ItemType = UItemType::EVENT;
}

void ACItem_Event::BeginPlay()
{
	Super::BeginPlay();
}

void ACItem_Event::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
