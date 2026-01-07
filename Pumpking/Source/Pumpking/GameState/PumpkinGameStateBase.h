// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PumpkinGameStateBase.generated.h"

/**
 *
 */
UCLASS()
class PUMPKING_API APumpkinGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable) void Multi_SendMessage(const FText& _text, APlayerState* _sender);
};
