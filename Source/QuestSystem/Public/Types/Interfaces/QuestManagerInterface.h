#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestManagerInterface.generated.h"

class UQuestManagerComponent;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UQuestManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class QUESTSYSTEM_API IQuestManagerInterface
{
	GENERATED_BODY()

public:
	virtual UQuestManagerComponent* GetQuestManagerComponent() const = 0;
};

