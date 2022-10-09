#include "Types/Objective.h"

#include "Core/QuestManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

UObjective::UObjective()
{
}

void UObjective::InitializeObjectives()
{
	const auto Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestManagerSubsystem>();

	for (auto Index = 0; Index < Objectives.Num(); ++Index)
	{
		const auto ObjectiveClass = Objectives[Index];
		if (!ObjectiveClass) continue;
		const auto NewObjective = Subsystem->CreateTask<UObjective>(ObjectiveClass, this);

		// Add to local cache
		ObjectivesInstance.Add(NewObjective);

		if (RequireOrder && Index > 0) continue;
		// Activate all if order not required
		NewObjective->TaskStatusChanged.AddDynamic(this, &UObjective::OnTaskStatusChanged);
		NewObjective->BeginTask();
	}
}

bool UObjective::ActivateNextObjective()
{
	if (!RequireOrder) return false;

	const auto FindTask = ObjectivesInstance.FindByPredicate([=](const UTask* Item) { return !Item->IsEnded(); });
	if (!FindTask) return false;

	const auto Task = *FindTask;
	Task->TaskStatusChanged.AddDynamic(this, &UObjective::OnTaskStatusChanged);
	Task->BeginTask();

	return true;
}

void UObjective::OnTaskStatusChanged(const UTask* TargetTask, ETaskStatus NewStatus)
{
	if (!TargetTask->IsEnded()) return;
	if (NewStatus == ETaskStatus::Failed && !CanFailed) return SetStatus(ETaskStatus::Failed);

	if (RequireOrder)
	{
		if (!ActivateNextObjective()) return SetStatus(ETaskStatus::Completed);
	}
	else
	{
		const auto IsNotEnded = ObjectivesInstance.ContainsByPredicate([](const UTask* Item)
		{
			return !Item->IsEnded();
		});
		if (!IsNotEnded) return SetStatus(ETaskStatus::Completed);
	}
}

void UObjective::BeginTask()
{
	InitializeObjectives();
	Super::BeginTask();
}

const TArray<UObjective*>& UObjective::GetObjectives() const
{
	return ObjectivesInstance;
}

UObjective* UObjective::GetObjectiveById(const FGameplayTag ObjectiveId) const
{
	return *ObjectivesInstance.FindByPredicate([=](const UObjective* Item) { return Item->Id == ObjectiveId; });
}

// Save & Load
void UObjective::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// Save Game
	if (Ar.IsSaveGame())
	{
		if (Ar.IsSaving()) OnSaveGame(Ar);
		else OnLoadGame(Ar);
	}
}

void UObjective::OnSaveGame(FArchive& Ar)
{
	// Objectives 
	int32 ObjectivesInstanceCount = ObjectivesInstance.Num();
	Ar << ObjectivesInstanceCount;

	for (const auto Item : ObjectivesInstance)
	{
		TSubclassOf<UObjective> ItemClass = Item->GetClass();
		Ar << ItemClass;

		FGameplayTag ObjectiveId = Item->Id;
		Ar << ObjectiveId;

		Item->Serialize(Ar);
	}
}

void UObjective::OnLoadGame(FArchive& Ar)
{
	// Objectives 
	int32 ObjectivesInstanceCount;
	Ar << ObjectivesInstanceCount;

	for (auto i = 0; i < ObjectivesInstanceCount; ++i)
	{
		TSubclassOf<UObjective> ItemClass;
		Ar << ItemClass;

		FGameplayTag ObjectiveId;
		Ar << ObjectiveId;

		// Objective every time exist when quest is started
		UObjective* Objective = GetObjectiveById(ObjectiveId);
		Objective->Serialize(Ar);
	}
}
