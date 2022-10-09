#pragma once

#include "CoreMinimal.h"
#include "Types/Interfaces/QuestManagerInterface.h"
#include "QuestManagerHelpers.generated.h"

UCLASS()
class QUESTSYSTEM_API UQuestManagerHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UQuestManagerComponent* GetQuestManagerComponent(const UObject* WorldContextObject, const UObject* Target,
	                                                        bool& IsFind);
};
