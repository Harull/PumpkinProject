// Copyright Epic Games, Inc. All Rights Reserved.


#include "PumpkingPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PumpkingCameraManager.h"

APumpkingPlayerController::APumpkingPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APumpkingCameraManager::StaticClass();
}

void APumpkingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
