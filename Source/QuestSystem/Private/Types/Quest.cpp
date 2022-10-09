#include "Types/Quest.h"

#include "Core/QuestManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Types/Objective.h"

UQuest::UQuest()
{
}

void UQuest::InitializeRootObjective()
{
	if (!RootObjective) return;

	const auto Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestManagerSubsystem>();

	RootObjectiveInstance = Subsystem->CreateTask<UObjective>(RootObjective, this);
	RootObjectiveInstance->TaskStatusChanged.AddDynamic(this, &ThisClass::OnTaskStatusChanged);
	RootObjectiveInstance->BeginTask();
}

void UQuest::OnTaskStatusChanged(const UTask* TargetTask, ETaskStatus NewStatus)
{
	SetStatus(NewStatus);
}

void UQuest::BeginTask()
{
	InitializeRootObjective();
	Super::BeginTask();
}

const UObjective* UQuest::GetRootObjective() const
{
	return RootObjectiveInstance;
}

const EQuestType& UQuest::GetQuestType() const
{
	return QuestType;
}

// Save & Load
void UQuest::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// Save Game
	if (Ar.IsSaveGame())
	{
		if (Ar.IsSaving()) OnSaveGame(Ar);
		else OnLoadGame(Ar);
	}
}

void UQuest::OnSaveGame(FArchive& Ar)
{
	bool HasRootObjective = !!RootObjectiveInstance;
	Ar << HasRootObjective;

	if (HasRootObjective)
		RootObjectiveInstance->Serialize(Ar);
}

void UQuest::OnLoadGame(FArchive& Ar)
{
	bool HasRootObjective;
	Ar << HasRootObjective;

	if (HasRootObjective)
		RootObjectiveInstance->Serialize(Ar);
}
