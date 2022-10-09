#include "Types/Task.h"

void UTask::BeginTask()
{
	if (Status != ETaskStatus::None) return;
	SetStatus(ETaskStatus::Activated);
}

bool UTask::IsEnded() const
{
	return Status == ETaskStatus::Completed || Status == ETaskStatus::Failed;
}

void UTask::SetStatus(const ETaskStatus NewStatus)
{
	if (Status == NewStatus) return;

	Status = NewStatus;
	TaskStatusChanged.Broadcast(this, Status);
}

void UTask::CompleteTask_Implementation(const FName& Argument)
{
	SetStatus(ETaskStatus::Completed);
}

void UTask::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);

	// Save Game
	if (Ar.IsSaveGame())
	{
		Ar << Status;
	}
}
