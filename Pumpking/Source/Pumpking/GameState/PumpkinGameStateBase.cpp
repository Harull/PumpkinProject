// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "PlayerController/PlayerCharacterController.h"
#include "Pumpking/Player/PlayerCharacter.h"
#include <Macro.h>
#include <Subsystem/RoomSubsystem.h>

void APumpkinGameStateBase::Multi_SendMessage_Implementation(const FText& _text, APlayerState* _sender)
{
	for (APlayerState* _playerState : PlayerArray)
	{
		if (!_playerState) continue;
		if (CAST(APlayerCharacterController, _playerController, _playerState->GetPlayerController()))
		{
			const FString& _playerName = _playerState->GetName();
			_playerController->ReceiveTchatMessage(_text, _sender);
		}
	}
}

void APumpkinGameStateBase::SetOcclusionTimer()
{
	for (APlayerState* _playerState : PlayerArray)
	{
		if (!_playerState) continue;

		if (CAST(APlayerCharacter, _player, _playerState->GetPawn()))
		{
			_player->Server_AskForNewPos();
		}
	}
	if (CAST(APlayerCharacter, _player, GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		LOG("GameStateClose");
		_player->Server_CloseLoadingScreen();
	}
	TIMER_LOOP(this, updateTimer, OnUpdateRoom(), 0.0f, 0.1f);
}

void APumpkinGameStateBase::OpenLoadingScreen()
{
	if (CAST(APlayerCharacter, _player, GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		LOG("GameStateClose");
		_player->Server_OpenLoadingScreen();
	}
}

void APumpkinGameStateBase::OnUpdateRoom()
{
	TObjectPtr<UWorld> _world = GetWorld();
	if (!_world) return;
	TObjectPtr<URoomSubsystem> _sub = _world->GetSubsystem<URoomSubsystem>();
	if (!_sub) return;
	_sub->UpdateRoomsForAllPlayers(PlayerArray);
}

void APumpkinGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) return;
}