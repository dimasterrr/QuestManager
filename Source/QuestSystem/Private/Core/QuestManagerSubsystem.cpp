#include "Core/QuestManagerSubsystem.h"
#include "Types/Task.h"

template <class T>
T* UQuestManagerSubsystem::CreateTask(const TSubclassOf<UTask> Class, UObject* Owner)
{
	const auto NewTask = NewObject<T>(Owner ? Owner : this, Class);
	TasksCache.Add(NewTask);

	return NewTask;
}

UTask* UQuestManagerSubsystem::CreateTask(const TSubclassOf<UTask> Class, UObject* Owner)
{
	return CreateTask<UTask>(Class, Owner);
}

bool UQuestManagerSubsystem::DestroyTask(const UTask* Task)
{
	if (!Task) return false;
	return DestroyTaskById(Task->Id);
}

bool UQuestManagerSubsystem::DestroyTaskById(const FGameplayTag Id)
{
	return TasksCache.RemoveAll([=](const auto Item) { return Item->Id == Id; }) > 0;
}

void UQuestManagerSubsystem::DestroyAllTasks()
{
	TasksCache.Empty();
}

const TArray<UTask*>& UQuestManagerSubsystem::GetAllTasks() const
{
	return TasksCache;
}

TArray<UTask*> UQuestManagerSubsystem::GetAllTasksByClass(const TSubclassOf<UTask> Class) const
{
	return TasksCache.FilterByPredicate([=](const UTask* Item) { return Item->GetClass() == Class; });
}

TArray<UTask*> UQuestManagerSubsystem::GetAllTasksByTag(const FGameplayTag Id) const
{
	return TasksCache.FilterByPredicate([=](const UTask* Item) { return Item->Id == Id; });
}

UTask* UQuestManagerSubsystem::GetTaskByTag(const FGameplayTag Id) const
{
	const auto Task = TasksCache.FindByPredicate([=](const UTask* Item) { return Item->Id == Id; });
	return Task ? *Task : nullptr;
}

// void UQuestManagerSubsystem::Serialize(FArchive& Ar)
// {
// 	Super::Serialize(Ar);
//
// 	// Save Game
// 	if (Ar.IsSaveGame())
// 	{
// 		if (Ar.IsSaving()) OnSaveGame(Ar);
// 		else OnLoadGame(Ar);
// 	}
// }
//
// void UQuestManagerSubsystem::OnSaveGame(FArchive& Ar)
// {
// 	int32 TasksCacheNumber = TasksCache.Num();
// 	Ar << TasksCacheNumber;
//
// 	for (const auto Item : TasksCache)
// 		Item->Serialize(Ar);
// }
//
// void UQuestManagerSubsystem::OnLoadGame(FArchive& Ar)
// {
// 	int32 TasksCacheNumber;
// 	Ar << TasksCacheNumber;
//
// 	for (auto i = 0; i < TasksCacheNumber; ++i)
// 	{
// 		UTask* Task = nullptr;
// 		Task->Serialize(Ar);
//
// 		TasksCache.AddUnique(Task);
// 	}
// }
