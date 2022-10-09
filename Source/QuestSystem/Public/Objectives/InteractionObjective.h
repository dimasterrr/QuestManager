#pragma once

#include "CoreMinimal.h"
#include "Types/Objective.h"
#include "InteractionObjective.generated.h"

UCLASS()
class QUESTSYSTEM_API UInteractionObjective final : public UObjective
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	FName UniqueActorId;

public:
	virtual void CompleteTask_Implementation(const FName& Argument) override;
};
