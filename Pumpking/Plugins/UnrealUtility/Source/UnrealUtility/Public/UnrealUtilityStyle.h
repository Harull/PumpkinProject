#pragma once

#include "Styling/SlateStyle.h"

class FUnrealUtilityStyle
{
	static TSharedPtr<class FSlateStyleSet> StyleInstance;

public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();
};