#include "Helpers/QuestManagerHelpers.h"

UQuestManagerComponent* UQuestManagerHelpers::GetQuestManagerComponent(const UObject* WorldContextObject,
                                                                       const UObject* Target,
                                                                       bool& IsFind)
{
	if (const auto Object = Cast<IQuestManagerInterface>(Target))
	{
		IsFind = true;
		return Object->GetQuestManagerComponent();
	}

	IsFind = false;
	return nullptr;
}
