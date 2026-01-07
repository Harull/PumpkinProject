// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterController.h"
#include <Macro.h>
#include <GameState/PumpkinGameStateBase.h>

APlayerCharacterController::APlayerCharacterController()
{
	bReplicates = true;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() && tchatType)
	{
		tchat = CreateWidget<UUW_Tchat>(this, tchatType);
		if (tchat)
		{
			tchat->AddToViewport();
			tchat->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void APlayerCharacterController::SendTchatMessage(const FText& _text)
{
	if (_text.IsEmpty()) return;
	Server_SendMessage(_text);
}

void APlayerCharacterController::ReceiveTchatMessage(const FText& _text, APlayerState* _sender)
{
	if (IsLocalController() && tchat)
	{
		tchat->AddText(_text, _sender == PlayerState);
	}
}

void APlayerCharacterController::ToggleTchat()
{
	if (!tchat) return;
	tchat->ToggleTchat();
}

void APlayerCharacterController::Server_SendMessage_Implementation(const FText& _text)
{
	if (CAST(APumpkinGameStateBase, _gameState, GetWorld()->GetGameState()))
	{
		_gameState->Multi_SendMessage(_text, PlayerState);
	}
}