#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "Objective.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class QUESTSYSTEM_API UObjective : public UTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Settings, meta=(AllowPrivateAccess=true))
	TArray<UObjective*> ObjectivesInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	bool RequireOrder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	bool CanFailed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	TArray<TSubclassOf<UObjective>> Objectives;

private:
	void InitializeObjectives();
	bool ActivateNextObjective();

protected:
	UFUNCTION(BlueprintCallable)
	void OnTaskStatusChanged(const UTask* TargetTask, ETaskStatus NewStatus);

	void OnSaveGame(FArchive& Ar);
	void OnLoadGame(FArchive& Ar);
	
public:
	UObjective();

	UFUNCTION(BlueprintCallable, Category=QuestManager)
	virtual void BeginTask() override;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	const TArray<UObjective*>& GetObjectives() const;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	UObjective* GetObjectiveById(const FGameplayTag ObjectiveId) const;

	virtual void Serialize(FArchive& Ar) override;
};
