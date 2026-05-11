#include "UnrealUtility.h"
#include "UnrealUtilityStyle.h"
#include "UnrealUtilityCommands.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "EditorModeManager.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "TickableEditorObject.h"
#include "Engine/Blueprint.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Settings/LevelEditorMiscSettings.h"
#include "UnrealUtilitySettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"

#include "Widgets/SWindow.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "EditorViewportClient.h"

static const FName UnrealUtilityTabName("UnrealUtility");

#define LOCTEXT_NAMESPACE "FUnrealUtilityModule"

void FUnrealUtilityModule::StartupModule()
{
	FUnrealUtilityStyle::Initialize();
	FUnrealUtilityStyle::ReloadTextures();

	FEditorDelegates::PostPIEStarted.AddRaw(this, &FUnrealUtilityModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FUnrealUtilityModule::OnEndPIE);

	FUnrealUtilityCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(FUnrealUtilityCommands::Get().RestartAction, FExecuteAction::CreateRaw(this, &FUnrealUtilityModule::RestartEditorClicked), FCanExecuteAction());
	PluginCommands->MapAction(FUnrealUtilityCommands::Get().CleanRebuildAction, FExecuteAction::CreateRaw(this, &FUnrealUtilityModule::CleanRebuildRestart), FCanExecuteAction());
	PluginCommands->MapAction(FUnrealUtilityCommands::Get().TeleportAction, FExecuteAction::CreateRaw(this, &FUnrealUtilityModule::TeleportMenu), FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealUtilityModule::RegisterMenus));

	if (ISettingsModule* _settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		_settingsModule->RegisterSettings(
			"Editor",
			"Unreal Utility",
			"Sounds",
			LOCTEXT("UnrealUtilityName", "Sounds"),
			LOCTEXT("UnrealUtilityDesc", "Audio settings for Unreal Utility."),
			GetMutableDefault<UUnrealUtilitySettings>()
		);
	}
}

void FUnrealUtilityModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		FEditorDelegates::BeginPIE.RemoveAll(this);
		FEditorDelegates::EndPIE.RemoveAll(this);
	}
	if (ISettingsModule* _settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		_settingsModule->UnregisterSettings("Editor", "Unreal Utility", "Sounds");
	}
}

void FUnrealUtilityModule::RegisterMenus()
{
	FToolMenuOwnerScoped _ownerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUnrealUtilityCommands::Get().RestartAction, PluginCommands);
			Section.AddMenuEntryWithCommandList(FUnrealUtilityCommands::Get().CleanRebuildAction, PluginCommands);
			Section.AddMenuEntryWithCommandList(FUnrealUtilityCommands::Get().TeleportAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealUtilityCommands::Get().RestartAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealUtilityCommands::Get().CleanRebuildAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealUtilityCommands::Get().TeleportAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FUnrealUtilityModule::RestartEditorClicked()
{
	FUnrealEdMisc::Get().RestartEditor();
}

void FUnrealUtilityModule::CleanRebuildRestart()
{
	const bool _shouldUseUBA = true;

	FString _projectFile = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	FString _projectDir = FPaths::GetPath(_projectFile);
	FString _engineExe = FPlatformProcess::ExecutablePath();
	FString _scriptPath = _projectDir / TEXT("RebuildAndRestart.bat");

	FString _ubaFlag = _shouldUseUBA ? TEXT("") : TEXT(" -NoUBA");

	FString _scriptContent = FString::Printf(TEXT(
		//"@echo off\n"
		//"setlocal\n"
		//"echo ======================================\n"
		//"echo Unreal Project Full Rebuild Script\n"
		//"echo ======================================\n"
		//"\n"
		//"echo Waiting for Unreal Editor to close...\n"
		//":wait_for_editor\n"
		//"tasklist /FI \"IMAGENAME eq UnrealEditor.exe\" | find /I \"UnrealEditor.exe\" >nul\n"
		//"if %%errorlevel%%==0 (\n"
		//" timeout /t 2 >nul\n"
		//" goto wait_for_editor\n"
		//")\n"
		//"echo Unreal Editor closed.\n"
		//"\n"
		//"echo.\n"
		//"echo Deleting old build folders...\n"

		//"rmdir /s /q \"%s\\Binaries\\Win64\"\n"
		//"rmdir /s /q \"%s\\Intermediate\\Build\"\n"
		//"rmdir /s /q \"%s\\Intermediate\\ProjectFiles\"\n"

		////"rmdir /s /q \"%s\\Intermediate\"\n"
		////"rmdir /s /q \"%s\\Binaries\"\n"

		//"del /q \"%s\\*.sln\" >nul 2>&1\n"

		//"\n"
		//"echo.\n"
		//"echo Regenerating Visual Studio project files...\n"

		//"set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe\"\n"
		//"if not exist %%UBT_PATH%% set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool.exe\"\n"
		//"if not exist %%UBT_PATH%% (\n"
		//" echo [ERROR] UnrealBuildTool.exe not found!\n"
		//" pause\n"
		//" exit /b 1\n"
		//")\n"
		//"%%UBT_PATH%% -projectfiles -project=\"%s\" -game -engine -progress\n"
		//"if %%errorlevel%% neq 0 (\n"
		//" echo [ERROR] Failed to generate project files.\n"
		//" pause\n"
		//" exit /b %%errorlevel%%\n"
		//")\n"
		//"\n"
		//"echo.\n"
		//"echo Compiling project...\n"
		//"%%UBT_PATH%% %sEditor Win64 Development -project=\"%s\" -progress -NoHotReloadFromIDE -DisableUnity%s\n"
		////"%%UBT_PATH%% %sEditor Win64 Development -project=\"%s\" -progress -ModuleWithSuffix=%s,Editor\n -SkipBuildPlugins -NoHotReloadFromIDE%s\n"
		//"if %%errorlevel%% neq 0 (\n"
		//" echo [ERROR] Build failed.\n"
		//" pause\n"
		//" exit /b %%errorlevel%%\n"
		//")\n"
		//"\n"
		//"echo.\n"
		//"echo Launching Unreal Editor...\n"
		//"start \"\" \"%s\" \"%s\" -Build\n"
		//"\n"
		//"echo.\n"
		//"echo All tasks completed successfully!\n"
		//"pause\n"
		//"endlocal\n"
		"@echo off\n"
		"setlocal\n"
		"echo ======================================\n"
		"echo Unreal Project Full Rebuild Script\n"
		"echo ======================================\n"
		"\n"
		"echo Waiting for Unreal Editor to close...\n"
		":wait_for_editor\n"
		"tasklist /FI \"IMAGENAME eq UnrealEditor.exe\" | find /I \"UnrealEditor.exe\" >nul\n"
		"if %%errorlevel%%==0 (\n"
		" timeout /t 2 >nul\n"
		" goto wait_for_editor\n"
		")\n"
		"echo Unreal Editor closed.\n"
		"\n"
		"echo.\n"
		"echo Deleting old build folders...\n"

		//"rmdir /s /q \"%s\\Binaries\\Win64\"\n"
		//"rmdir /s /q \"%s\\Intermediate\\Build\"\n"
		//"rmdir /s /q \"%s\\Intermediate\\ProjectFiles\"\n"

		"rmdir /s /q \"%s\\Binaries\"\n"
		"rmdir /s /q \"%s\\Intermediate\"\n"
		"del /q \"%s\\*.sln\" >nul 2>&1\n"

		"\n"
		"echo.\n"
		"echo Regenerating Visual Studio project files...\n"

		"set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe\"\n"
		"if not exist %%UBT_PATH%% set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool.exe\"\n"
		"if not exist %%UBT_PATH%% (\n"
		" echo [ERROR] UnrealBuildTool.exe not found!\n"
		" pause\n"
		" exit /b 1\n"
		")\n"
		"%%UBT_PATH%% -projectfiles -project=\"%s\" -game -engine -progress\n"
		"if %%errorlevel%% neq 0 (\n"
		" echo [ERROR] Failed to generate project files.\n"
		" pause\n"
		" exit /b %%errorlevel%%\n"
		")\n"
		"\n"
		"echo.\n"
		"echo Compiling project...\n"
		"%%UBT_PATH%% %sEditor Win64 Development -project=\"%s\" -progress\n"
		//"%%UBT_PATH%% %sEditor Win64 Development -project=\"%s\" -progress -ModuleWithSuffix=%s,Editor\n -SkipBuildPlugins -NoHotReloadFromIDE%s\n"
		"if %%errorlevel%% neq 0 (\n"
		" echo [ERROR] Build failed.\n"
		" pause\n"
		" exit /b %%errorlevel%%\n"
		")\n"
		"\n"
		"echo.\n"
		"echo Launching Unreal Editor...\n"
		"start \"\" \"%s\" \"%s\" -Build\n"
		"\n"
		"echo.\n"
		"echo All tasks completed successfully!\n"
		"pause\n"
		"endlocal\n"
	),
		*_projectDir,
		*_projectDir,
		//*_projectDir,
		*_projectDir,
		*FPaths::RootDir(),
		*FPaths::RootDir(),
		*_projectFile,
		*FPaths::GetBaseFilename(_projectFile),
		*_projectFile,
		//*_ubaFlag,
		*_engineExe,
		*_projectFile);
	FFileHelper::SaveStringToFile(_scriptContent, *_scriptPath);
	FPlatformProcess::CreateProc(*_scriptPath, nullptr, false, false, false, nullptr, 0, *_projectDir, nullptr);
	FGenericPlatformMisc::RequestExit(false);
}

void FUnrealUtilityModule::TeleportMenu()
{
	if (!GEditor)
		return;

	TSharedPtr<SEditableTextBox> _xBox;
	TSharedPtr<SEditableTextBox> _yBox;
	TSharedPtr<SEditableTextBox> _zBox;

	TSharedRef<SWindow> _window = SNew(SWindow).Title(FText::FromString("Teleport")).ClientSize(FVector2D(260, 160)).SupportsMinimize(false).SupportsMaximize(false);
	_window->SetContent(SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(5)[SAssignNew(_xBox, SEditableTextBox).HintText(FText::FromString("X"))]
		+ SVerticalBox::Slot().Padding(5)[SAssignNew(_yBox, SEditableTextBox).HintText(FText::FromString("Y"))]
		+ SVerticalBox::Slot().Padding(5)[SAssignNew(_zBox, SEditableTextBox).HintText(FText::FromString("Z"))]
		+ SVerticalBox::Slot().Padding(10).HAlign(HAlign_Right)[SNew(SButton).Text(FText::FromString("Teleport")).OnClicked_Lambda([_xBox, _yBox, _zBox, _window]() {
			const float& _x = FCString::Atof(*_xBox->GetText().ToString());
			const float& _y = FCString::Atof(*_yBox->GetText().ToString());
			const float& _z = FCString::Atof(*_zBox->GetText().ToString());
			if (GEditor && GEditor->GetActiveViewport())
			{
				if (FEditorViewportClient* _viewport = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient()))
				{
					_viewport->SetViewLocation(FVector(_x, _y, _z));
					_viewport->Invalidate();
				}
			}
			FSlateApplication::Get().RequestDestroyWindow(_window);
			return FReply::Handled();
		})]
	);
	FSlateApplication::Get().AddWindow(_window);
}

void FUnrealUtilityModule::OnBeginPIE(bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("Started"));
	FTimerHandle TimerHandle;
	GEditor->GetTimerManager()->SetTimer(TimerHandle, [this]()
	{
		PlayEditorSound(TEXT("/Script/Engine.SoundWave'/Engine/EditorSounds/GamePreview/StartPlayInEditor.StartPlayInEditor'"), false);
	}, 0.1f, false);
}

void FUnrealUtilityModule::OnEndPIE(bool bIsSimulating)
{
	PlayEditorSound(TEXT("/Script/Engine.SoundWave'/Engine/EditorSounds/GamePreview/EndPlayInEditor.EndPlayInEditor'"), true);
}

void FUnrealUtilityModule::PlayEditorSound(const FString& Path, const bool& _isEditorSound)
{
	const UUnrealUtilitySettings* _settings = GetDefault<UUnrealUtilitySettings>();
	if (!_settings->GetRestoreEditorSounds())
		return;

	USoundBase* _sound = LoadObject<USoundBase>(nullptr, *Path);
	if (!_sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EditorSoundsRestored] Failed to load sound: %s"), *Path);
		return;
	}

	if (_isEditorSound)
		GEditor->PlayEditorSound(_sound);
	else
		UGameplayStatics::PlaySound2D(GEditor->PlayWorld, _sound);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealUtilityModule, UnrealUtility)