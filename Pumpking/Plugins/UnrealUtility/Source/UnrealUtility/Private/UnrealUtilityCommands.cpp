#include "UnrealUtilityCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealUtilityModule"

void FUnrealUtilityCommands::RegisterCommands()
{
	UI_COMMAND(RestartAction, "Restart Editor", "Restart the editor and reopen the current project.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CleanRebuildAction, "Clean Rebuild Project", "Deletes Binaries/Intermediate and reopen project.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(TeleportAction, "Teleport", "Teleports to a location.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
