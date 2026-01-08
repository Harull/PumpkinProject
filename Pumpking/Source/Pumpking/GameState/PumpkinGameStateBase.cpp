// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "PlayerController/PlayerCharacterController.h"
#include <Macro.h>

void APumpkinGameStateBase::Multi_SendMessage_Implementation(const FText& _text, APlayerState* _sender)
{
	for (APlayerState* _playerState : PlayerArray)
	{
		if (!_playerState) continue;
		if (CAST(APlayerCharacterController, _playerController, _playerState->GetPlayerController()))
		{
			const FString& _playerName = _playerState->GetName();
			FText _newText = _text;
			_newText = FText::FromString(_playerName + " :\r" + _text.ToString());
			_playerController->ReceiveTchatMessage(_text, _sender);
		}
	}
}