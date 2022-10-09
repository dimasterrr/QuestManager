#pragma once

#include "CoreMinimal.h"
#include "Types/Objective.h"
#include "CollectObjective.generated.h"

UCLASS()
class QUESTSYSTEM_API UCollectObjective : public UObjective
{
	GENERATED_BODY()

private:
	int32 CurrentCount;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	FName UniqueItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	int32 Count;

public:
	UFUNCTION(BlueprintPure, Category="QuestManager|Objectives")
	int32 GetCurrentCount() const;

	virtual void CompleteTask_Implementation(const FName& Argument) override;
};
