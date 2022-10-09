#pragma once

#include "CoreMinimal.h"
#include "Types/Objective.h"
#include "LocationObjective.generated.h"

UCLASS()
class QUESTSYSTEM_API ULocationObjective final : public UObjective
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	FName UniqueLocationId;

public:
	virtual void CompleteTask_Implementation(const FName& Argument) override;
};
