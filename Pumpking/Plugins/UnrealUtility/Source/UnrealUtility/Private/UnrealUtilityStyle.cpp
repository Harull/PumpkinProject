#include "UnrealUtilityStyle.h"
#include "UnrealUtility.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FUnrealUtilityStyle::StyleInstance = nullptr;

void FUnrealUtilityStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FUnrealUtilityStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FUnrealUtilityStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UnrealUtilityStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FUnrealUtilityStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("UnrealUtilityStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("UnrealUtility")->GetBaseDir() / TEXT("Resources"));

	Style->Set("UnrealUtility.RestartAction", new IMAGE_BRUSH_SVG(TEXT("RestartIcon"), Icon20x20));
	Style->Set("UnrealUtility.CleanRebuildAction", new IMAGE_BRUSH_SVG(TEXT("CleanRebuildIcon"), Icon20x20));
	Style->Set("UnrealUtility.TeleportAction", new IMAGE_BRUSH_SVG(TEXT("TeleportIcon"), Icon20x20));
	return Style;
}

void FUnrealUtilityStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUnrealUtilityStyle::Get()
{
	return *StyleInstance;
}
