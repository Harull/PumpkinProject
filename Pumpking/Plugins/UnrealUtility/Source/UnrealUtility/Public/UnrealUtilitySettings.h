#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UnrealUtilitySettings.generated.h"

UCLASS(config = EditorPerUserSettings, defaultconfig, meta = (DisplayName = "UnrealUtility Settings"))
class UUnrealUtilitySettings : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, config, Category = "Sounds") bool restoreEditorSounds = false;

public:
	FORCEINLINE bool GetRestoreEditorSounds() const { return restoreEditorSounds; }

public:
	UUnrealUtilitySettings();
};
