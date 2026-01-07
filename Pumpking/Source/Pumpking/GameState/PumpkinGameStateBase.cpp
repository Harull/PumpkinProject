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
		LOG(_playerState->GetName());
		if (CAST(APlayerCharacterController, _playerController, _playerState->GetPlayerController()))
		{
			_playerController->ReceiveTchatMessage(_text, _sender);
		}
	}
}