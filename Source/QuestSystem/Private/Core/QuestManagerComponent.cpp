#include "Core/QuestManagerComponent.h"

#include "Core/QuestManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UQuestManagerComponent::AddQuest(UQuest* Quest)
{
	if (!Quest) return;

	AcceptedQuests.AddUnique(Quest);
	QuestAccepted.Broadcast(Quest);

	if (!ActiveQuest) SetActiveQuest(Quest);
}

const TArray<UQuest*>& UQuestManagerComponent::GetQuests()
{
	return AcceptedQuests;
}

TArray<UQuest*> UQuestManagerComponent::GetQuestsByType(const EQuestType Type) const
{
	return AcceptedQuests.FilterByPredicate([=](const UQuest* Item) { return Item->GetQuestType() == Type; });
}

UQuest* UQuestManagerComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

void UQuestManagerComponent::SetActiveQuest(UQuest* Quest)
{
	if (!AcceptedQuests.Contains(Quest)) return;
	if (Quest->IsEnded()) return;

	const auto OldQuest = ActiveQuest;

	ActiveQuest = Quest;
	QuestActivated.Broadcast(OldQuest, ActiveQuest);
}

void UQuestManagerComponent::SetActiveQuestById(FGameplayTag QuestId)
{
	const auto Quest = AcceptedQuests.FindByPredicate([=](const auto Item) { return Item->Id == QuestId; });
	if (!Quest) return;

	SetActiveQuest(*Quest);
}

UQuest* UQuestManagerComponent::GetQuestById(FGameplayTag QuestId) const
{
	if (!HasAcceptedQuestById(QuestId)) return nullptr;
	return *AcceptedQuests.FindByPredicate([=](const auto Item) { return Item->Id == QuestId; });
}

UQuest* UQuestManagerComponent::GetQuestByClass(const TSubclassOf<UQuest> Quest) const
{
	if (!HasAcceptedQuest(Quest)) return nullptr;
	return *AcceptedQuests.FindByPredicate([=](const auto Item) { return Item->GetClass() == Quest; });
}

bool UQuestManagerComponent::HasAcceptedQuest(const TSubclassOf<UQuest> Quest) const
{
	return AcceptedQuests.ContainsByPredicate([=](const UQuest* Item) { return Item->GetClass() == Quest; });
}

bool UQuestManagerComponent::HasAcceptedQuestById(const FGameplayTag Id) const
{
	return AcceptedQuests.ContainsByPredicate([=](const UQuest* Item) { return Item->Id == Id; });
}

bool UQuestManagerComponent::TryActivateNextQuest()
{
	if (ActiveQuest && !ActiveQuest->IsEnded()) return false;

	auto ActivatedQuests = AcceptedQuests.FilterByPredicate([](const UTask* Item) { return !Item->IsEnded(); });
	if (ActivatedQuests.IsEmpty())
	{
		ActiveQuest = nullptr;
		return false;
	}

	ActivatedQuests.Sort([](const UQuest& A, const UQuest& B) { return A.GetQuestType() < B.GetQuestType(); });

	const auto NextActivateTask = ActivatedQuests[0];
	SetActiveQuest(NextActivateTask);

	return true;
}

void UQuestManagerComponent::OnTaskStatusChanged(const UTask* Task, ETaskStatus Status)
{
	if (ActiveQuest != Task) return;
	if (!Task->IsEnded()) return;

	// Link task to quest && execute event
	if (const auto Quest = Cast<UQuest>(Task))
		QuestStatusChanged.Broadcast(Quest, Status, Status);

	// Start next quest
	TryActivateNextQuest();
}

// New Interface
UQuest* UQuestManagerComponent::BeginQuest(const TSubclassOf<UQuest> Quest)
{
	if (HasAcceptedQuest(Quest)) return nullptr;

	const auto Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestManagerSubsystem>();
	const auto NewQuest = Subsystem->CreateTask<UQuest>(Quest, this);
	NewQuest->TaskStatusChanged.AddDynamic(this, &UQuestManagerComponent::OnTaskStatusChanged);

	// Add to activated quests list
	AddQuest(NewQuest);

	// Start new quest 
	NewQuest->BeginTask();

	return NewQuest;
}

void UQuestManagerComponent::RestoreQuest(const TSubclassOf<UQuest> Quest)
{
	if (!HasAcceptedQuest(Quest)) return;
}

void UQuestManagerComponent::ForgotQuest(const TSubclassOf<UQuest> Quest)
{
	if (!HasAcceptedQuest(Quest)) return;
}

void UQuestManagerComponent::EndQuest(const TSubclassOf<UQuest> Quest)
{
	if (!HasAcceptedQuest(Quest)) return;
}

void UQuestManagerComponent::SetTaskStatus(const FGameplayTag Id, const ETaskStatus NewStatus) const
{
	const auto Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestManagerSubsystem>();
	const auto Task = Subsystem->GetTaskByTag(Id);
	Task->SetStatus(NewStatus);
}

void UQuestManagerComponent::CompleteTask(const TSubclassOf<UTask> Task, const FName Argument)
{
	const auto Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestManagerSubsystem>();
	const auto Tasks = Subsystem->GetAllTasksByClass(Task);

	for (const auto Item : Tasks)
		Item->CompleteTask(Argument);
}

// Save & Load
void UQuestManagerComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// Save Game
	if (Ar.IsSaveGame())
	{
		if (Ar.IsSaving()) OnSaveGame(Ar);
		else OnLoadGame(Ar);
	}
}

void UQuestManagerComponent::OnSaveGame(FArchive& Ar)
{
	// Accepted quests
	int32 QuestsCount = AcceptedQuests.Num();
	Ar << QuestsCount;

	for (const auto Item : AcceptedQuests)
	{
		// Save class name
		TSubclassOf<UQuest> ItemClass = Item->GetClass();
		Ar << ItemClass;

		// Save quest id
		FGameplayTag QuestId = Item->Id;
		Ar << QuestId;

		// Save object data
		Item->Serialize(Ar);
	}

	// Active quest
	bool HasActiveQuest = !!ActiveQuest;
	Ar << HasActiveQuest;

	if (HasActiveQuest)
	{
		Ar << ActiveQuest->Id;
	}
}

void UQuestManagerComponent::OnLoadGame(FArchive& Ar)
{
	const auto Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();
	// Reset prev data
	AcceptedQuests.Empty();
	Subsystem->DestroyAllTasks();

	// Accepted quests
	int32 QuestsCount;
	Ar << QuestsCount;

	for (auto i = 0; i < QuestsCount; ++i)
	{
		// Restore target class
		TSubclassOf<UQuest> ItemClass;
		Ar << ItemClass;

		// Restore quest id
		FGameplayTag QuestId;
		Ar << QuestId;

		// ReCreate quest
		UQuest* Quest = GetQuestById(QuestId);
		if (!Quest) Quest = BeginQuest(ItemClass);

		Quest->Serialize(Ar);
	}

	// Active quest
	bool HasActiveQuest;
	Ar << HasActiveQuest;

	if (HasActiveQuest)
	{
		FGameplayTag ActiveQuestId;
		Ar << ActiveQuestId;

		ActiveQuest = GetQuestById(ActiveQuestId);
	}
}
