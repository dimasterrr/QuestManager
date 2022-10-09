#include "Objectives/LocationObjective.h"

void ULocationObjective::CompleteTask_Implementation(const FName& Argument)
{
	if (Argument != UniqueLocationId) return;
	Super::CompleteTask_Implementation(Argument);
}
