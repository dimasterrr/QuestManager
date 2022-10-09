#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/Quest.h"
#include "QuestManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestAccepted, const UQuest*, Quest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestActivated, const UQuest*, OldQuest, UQuest*, NewQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuestStatusChanged, const UQuest*, Quest,
                                               ETaskStatus, OldStatus, ETaskStatus, NewStatus);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	TArray<UQuest*> AcceptedQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	UQuest* ActiveQuest;

public:
	UPROPERTY(BlueprintAssignable)
	FQuestAccepted QuestAccepted;

	UPROPERTY(BlueprintAssignable)
	FQuestActivated QuestActivated;

	UPROPERTY(BlueprintAssignable)
	FQuestStatusChanged QuestStatusChanged;

protected:
	void OnSaveGame(FArchive& Ar);
	void OnLoadGame(FArchive& Ar);
	
public:
	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void AddQuest(UQuest* Quest);

	UFUNCTION(BlueprintPure, Category=QuestComponent)
	const TArray<UQuest*>& GetQuests();

	UFUNCTION(BlueprintPure, Category=QuestComponent)
	TArray<UQuest*> GetQuestsByType(EQuestType Type) const;

	UFUNCTION(BlueprintPure, Category=QuestComponent)
	UQuest* GetActiveQuest() const;

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void SetActiveQuest(UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void SetActiveQuestById(FGameplayTag QuestId);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	UQuest* GetQuestById(FGameplayTag QuestId) const;

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	UQuest* GetQuestByClass(TSubclassOf<UQuest> Quest) const;

	UFUNCTION(BlueprintPure, Category=QuestComponent)
	bool HasAcceptedQuest(TSubclassOf<UQuest> Quest) const;

	UFUNCTION(BlueprintPure, Category=QuestComponent)
	bool HasAcceptedQuestById(FGameplayTag Id) const;
	bool TryActivateNextQuest();

	UFUNCTION(BlueprintCallable, Category=Develop)
	void OnTaskStatusChanged(const UTask* Task, ETaskStatus Status);
	
	// New interface
	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	UQuest* BeginQuest(TSubclassOf<UQuest> Quest);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void RestoreQuest(TSubclassOf<UQuest> Quest);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void ForgotQuest(TSubclassOf<UQuest> Quest);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void EndQuest(TSubclassOf<UQuest> Quest);

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void SetTaskStatus(const FGameplayTag Id, const ETaskStatus NewStatus) const;

	UFUNCTION(BlueprintCallable, Category=QuestComponent)
	void CompleteTask(const TSubclassOf<UTask> Task, const FName Argument);

	// Save & Load
	virtual void Serialize(FArchive& Ar) override;
};
