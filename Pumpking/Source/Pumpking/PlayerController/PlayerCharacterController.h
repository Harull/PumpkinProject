// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widget\UW_Tchat.h"
#include "PlayerCharacterController.generated.h"

/**
 *
 */
UCLASS()
class PUMPKING_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TSubclassOf<UUW_Tchat> tchatType = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UUW_Tchat> tchat = nullptr;

public:
	APlayerCharacterController();

protected:
	virtual void BeginPlay() override;

public:
	void SendTchatMessage(const FText& _text);
	void ReceiveTchatMessage(const FText& _text, APlayerState* _sender);
	void ToggleTchat();

protected:
	UFUNCTION(Server, Reliable) void Server_SendMessage(const FText& _text);
};
