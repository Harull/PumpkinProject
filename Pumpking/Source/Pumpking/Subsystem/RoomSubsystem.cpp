// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/RoomSubsystem.h"
#include "Pumpking/GenerationRelated/Room.h"
#include <Macro.h>

void URoomSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{
	FTimerHandle _timer;
	TIMER_LOOP(GetWorld(), _timer, UpdatelAllRooms(), 0.0f, 0.5f);
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

void URoomSubsystem::UpdatelAllRooms()
{
	if (allRooms.IsEmpty()) return;
	APawn* _player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!_player) return;
	const FVector& _playerPos = _player->GetActorLocation();

	const int _size = allRooms.Num();
	for (int _index = 0; _index < _size; _index++)
	{
		ARoom* _room = allRooms[_index];
		if (!_room) continue;
		const FVector& _roomPos = _room->GetActorLocation();
		const bool _enabled = FVector::Dist(_roomPos, _playerPos) < maxDist;
		_room->ChangeRoomState(_enabled);
	}
}