#include "Objectives/CollectObjective.h"

int32 UCollectObjective::GetCurrentCount() const
{
	return CurrentCount;
}

void UCollectObjective::CompleteTask_Implementation(const FName& Argument)
{
	if (Argument != UniqueItemId || CurrentCount < Count) return;
	Super::CompleteTask_Implementation(Argument);
}
