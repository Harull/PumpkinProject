#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FUnrealUtilityModule : public IModuleInterface
{
	TSharedPtr<class FUICommandList> PluginCommands;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// Init methods
private:
	void RegisterMenus();

	// Buttons interactions
private:
	void RestartEditorClicked();
	void CleanRebuildRestart();
	void TeleportMenu();

	// Events
private:
	void OnBeginPIE(bool bIsSimulating);
	void OnEndPIE(bool bIsSimulating);

	// Utility
private:
	void PlayEditorSound(const FString& Path, const bool& _isEditorSound);
};
