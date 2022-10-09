#pragma once

#include "CoreMinimal.h"
#include "Objective.h"
#include "Task.h"
#include "Quest.generated.h"

class UObjective;

/* Order == Priority */
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Story,
	Advanced
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class QUESTSYSTEM_API UQuest : public UTask
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category=Develop, meta=(AllowPrivateAccess=true))
	UObjective* RootObjectiveInstance;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	FGameplayTag PrerequisiteQuests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	EQuestType QuestType = EQuestType::Story;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	TSubclassOf<UObjective> RootObjective;

private:
	void InitializeRootObjective();

protected:
	UFUNCTION(BlueprintCallable)
	void OnTaskStatusChanged(const UTask* TargetTask, ETaskStatus NewStatus);

	void OnSaveGame(FArchive& Ar);
	void OnLoadGame(FArchive& Ar);
	
public:
	UQuest();

	UFUNCTION(BlueprintCallable, Category=QuestManager)
	virtual void BeginTask() override;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	const UObjective* GetRootObjective() const;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	const EQuestType& GetQuestType() const;

	// Save & Load
	virtual void Serialize(FArchive& Ar) override;
};
