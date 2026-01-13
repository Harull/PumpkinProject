// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/RoomSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Pumpking/GenerationRelated/Room.h"
#include <Macro.h>

void URoomSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{
}

bool URoomSubsystem::IsRoomRelevantForPlayer(TObjectPtr<ARoom> _room, TObjectPtr<APawn> _pawn) const
{
	if (!_room || !_pawn) return false;

	const FVector _playerPos = _pawn->GetActorLocation();
	const FVector _roomPos = _room->GetActorLocation();

	if (_room->GetRoomCollision().IsInside(_playerPos))
		return true;

	const float _distSq = FVector::DistSquared(_playerPos, _roomPos);
	const float _nearDist = 2000.0f;
	if (_distSq < _nearDist * _nearDist)
		return true;

	const float _maxDistSq = maxDist * maxDist;
	if (_distSq > _maxDistSq)
		return false;

	const FVector _playerForward = _pawn->GetActorForwardVector();
	const FVector _dir = (_roomPos - _playerPos).GetSafeNormal();
	const float _dot = FVector::DotProduct(_playerForward, _dir);

	return _dot >= 0.4f;
}

void URoomSubsystem::AddRoom(TObjectPtr<ARoom> _newRoom)
{
	if (allRooms.Contains(_newRoom)) return;
	allRooms.Add(_newRoom);
}

void URoomSubsystem::RemoveRoom(TObjectPtr<ARoom> _newRoom)
{
	allRooms.Remove(_newRoom);
}

void URoomSubsystem::UpdateRoomsForAllPlayers(TArray<TObjectPtr<APlayerState>> _players)
{
	if (allRooms.IsEmpty()) return;
	const int _playersSize = _players.Num();
	const int _roomsSize = allRooms.Num();
	for (int _rIndex = 0; _rIndex < _roomsSize; _rIndex++)
	{
		TObjectPtr<ARoom> _room = allRooms[_rIndex];
		if (!_room) continue;
		bool _shouldBeEnabled = false;
		for (int _pIndex = 0; _pIndex < _playersSize; _pIndex++)
		{
			TObjectPtr<APlayerState> _playerState = _players[_pIndex];
			if (!_playerState) continue;
			TObjectPtr<APawn> _pawn = _playerState->GetPawn();
			if (!_pawn) continue;
			if (IsRoomRelevantForPlayer(_room, _pawn))
			{
				_shouldBeEnabled = true;
				break;
			}
		}
		_room->ChangeRoomState(_shouldBeEnabled);
	}
}