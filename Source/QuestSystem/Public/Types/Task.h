#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Task.generated.h"

UENUM(BlueprintType)
enum class ETaskStatus : uint8
{
	None,
	Activated,
	Completed,
	Failed
};

UCLASS(Abstract, NotBlueprintable)
class QUESTSYSTEM_API UTask : public UObject
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTaskStatusChanged, const UTask*, Task, ETaskStatus, Status);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	FGameplayTag Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	ETaskStatus Status = ETaskStatus::None;

	UPROPERTY(BlueprintAssignable, Category=Events)
	FTaskStatusChanged TaskStatusChanged;

public:
	virtual void BeginTask();

	UFUNCTION(BlueprintPure, Category=QuestManager)
	bool IsEnded() const;

	UFUNCTION(BlueprintCallable, Category=QuestManager)
	void SetStatus(const ETaskStatus NewStatus);
	
	UFUNCTION(BlueprintNativeEvent, Category=QuestManager)
	void CompleteTask(const FName& Argument);

	virtual void Serialize(FArchive& Ar) override;
};
