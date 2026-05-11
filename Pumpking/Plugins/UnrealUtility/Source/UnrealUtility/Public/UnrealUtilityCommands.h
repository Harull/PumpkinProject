#pragma once

#include "Framework/Commands/Commands.h"
#include "UnrealUtilityStyle.h"

class FUnrealUtilityCommands : public TCommands<FUnrealUtilityCommands>
{
public:
	TSharedPtr<FUICommandInfo> RestartAction;
	TSharedPtr<FUICommandInfo> CleanRebuildAction;
	TSharedPtr<FUICommandInfo> TeleportAction;

public:
	FUnrealUtilityCommands() : TCommands<FUnrealUtilityCommands>(TEXT("UnrealUtility"), NSLOCTEXT("Contexts", "UnrealUtility", "UnrealUtility Plugin"), NAME_None, FUnrealUtilityStyle::GetStyleSetName())
	{
	}
	virtual void RegisterCommands() override;
};
