#include "Objectives/InteractionObjective.h"

void UInteractionObjective::CompleteTask_Implementation(const FName& Argument)
{
	if (Argument != UniqueActorId) return;
	Super::CompleteTask_Implementation(Argument);
}

