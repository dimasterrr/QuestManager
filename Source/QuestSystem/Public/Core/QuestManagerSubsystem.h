#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestManagerSubsystem.generated.h"

class UTask;

UCLASS()
class QUESTSYSTEM_API UQuestManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Develop)
	TArray<UTask*> TasksCache;

// protected:
// 	void OnSaveGame(FArchive& Ar);
// 	void OnLoadGame(FArchive& Ar);
	
public:
	template<class T>
	T* CreateTask(const TSubclassOf<UTask> Class, UObject* Owner = nullptr);
	
	UFUNCTION(BlueprintCallable, Category=QuestManager)
	UTask* CreateTask(const TSubclassOf<UTask> Class, UObject* Owner = nullptr);

	UFUNCTION(BlueprintCallable, Category=QuestManager)
	bool DestroyTask(const UTask* Task);
	
	UFUNCTION(BlueprintCallable, Category=QuestManager)
	bool DestroyTaskById(const FGameplayTag Id);

	UFUNCTION(BlueprintCallable, Category=QuestManager)
	void DestroyAllTasks();

	UFUNCTION(BlueprintPure, Category=QuestManager)
	const TArray<UTask*>& GetAllTasks() const;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	TArray<UTask*> GetAllTasksByClass(const TSubclassOf<UTask> Class) const;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	TArray<UTask*> GetAllTasksByTag(const FGameplayTag Id) const;

	UFUNCTION(BlueprintPure, Category=QuestManager)
	UTask* GetTaskByTag(const FGameplayTag Id) const;

	// virtual void Serialize(FArchive& Ar) override;
};
