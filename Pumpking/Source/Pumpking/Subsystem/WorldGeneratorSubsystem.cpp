// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGeneratorSubsystem.h"
#include "Pumpking/GenerationRelated/Room.h"
#include "Pumpking/GenerationRelated/Door.h"
#include "Engine/OverlapResult.h"
#include <Macro.h>

TObjectPtr<ARoom> UWorldGeneratorSubsystem::GenerateNewRoom(const FVector& _position)
{
	TSubclassOf<ARoom> _roomType = GetRandomRoomPreset();
	if (!_roomType)
	{
		canGenerate = false;
		return nullptr;
	}
	FActorSpawnParameters _param;
	_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TObjectPtr<ARoom> _newRoom = GetWorld()->SpawnActor<ARoom>(_roomType, _position, FRotator::ZeroRotator, _param);
	if (!_newRoom)
	{
		return nullptr;
	}
	allRooms.Add(_newRoom);
	generatedRoomCount++;
	return _newRoom;
}

bool UWorldGeneratorSubsystem::ComputeNewPosForRoom(const FVector& _currentDoorPos, const FVector& _direction, TObjectPtr<ARoom> _newRoom)
{
	TObjectPtr<ADoor> _door = _newRoom->GetRandomAvailableDoor();
	if (!_door)
	{
		_newRoom->Destroy();
		generatedRoomCount--;
		return false;
	}

	FVector _toForward = _door->GetActorForwardVector();
	FQuat _deltaQuat = FQuat::FindBetweenVectors(_toForward, -_direction);
	FQuat _newRoomQuat = _deltaQuat * _newRoom->GetActorQuat();
	_newRoom->SetActorRotation(_newRoomQuat);

	const FVector& _localDoorPos = _newRoom->GetActorLocation() - _door->GetActorLocation();
	const FVector& _newDoorLoc = _currentDoorPos + _direction;
	_newRoom->SetActorLocation(_newDoorLoc + _localDoorPos);

	_newRoom->ComputeCollision();
	FBox _currentRoomCollision = _newRoom->GetRoomCollision();
	for (ARoom* _room : allRooms)
	{
		if (_newRoom == _room) continue;
		FBox _roomCollision = _room->GetRoomCollision();
		if (_currentRoomCollision.Intersect(_roomCollision))
		{
			allRooms.Remove(_newRoom);
			_newRoom->Destroy();
			return false;
		}
	}
	_door->SetIsConnected(true);
	allConnectedDoor.Add(_door);
	return true;
}

TSubclassOf<ARoom> UWorldGeneratorSubsystem::GetRandomRoomPreset()
{
	if (!data || data->allRoomsTypes.IsEmpty())
		return nullptr;
	const int _index = FMath::RandRange(0, data->allRoomsTypes.Num() - 1);
	return data->allRoomsTypes[_index];
}

void UWorldGeneratorSubsystem::GenerateFirstRoom()
{
	TObjectPtr<ARoom> _room = GenerateNewRoom(FVector::ZeroVector);
	if (!_room)
	{
		canGenerate = false;
		return;
	}
	_room->ComputeCollision();
	generatedRoomCount++;
}

void UWorldGeneratorSubsystem::GenerateAllRooms()
{
	while (canGenerate && (generatedRoomCount < data->maximumRoomCount))
	{
		TObjectPtr<ARoom> _room = GetRandomRoomWithAvailableDoor();
		if (!_room) break;

		TObjectPtr<ADoor> _door = _room->GetRandomAvailableDoor();
		if (!_door) continue;

		TObjectPtr<ARoom> _newRoom = GenerateNewRoom(_door->GetActorLocation());
		if (!_newRoom) continue;

		bool _isValid = ComputeNewPosForRoom(_door->GetActorLocation(), _door->GetActorForwardVector(), _newRoom);
		if (!_isValid)
			continue;
		_door->SetIsConnected(true);
		allConnectedDoor.Add(_door);
	}
}

void UWorldGeneratorSubsystem::OccludeAvailablesDoors()
{
	for (ARoom* _room : allRooms)
	{
		if (!_room->HasAvailableDoor()) continue;

		for (ADoor* _door : _room->GetAllAvailablesDoors())
		{
			_door->SetIsCondamned(true);
			allOccludedDoors.Add(_door);
		}
	}
}

TObjectPtr<ARoom> UWorldGeneratorSubsystem::GetRandomRoomWithAvailableDoor()
{
	TArray<TObjectPtr<ARoom>> _roomsWithAvailablesDoors;
	for (TObjectPtr<ARoom> _room : allRooms)
	{
		if (_room && _room->HasAvailableDoor())
		{
			_roomsWithAvailablesDoors.Add(_room);
		}
	}
	if (_roomsWithAvailablesDoors.IsEmpty())
	{
		canGenerate = false;
		return nullptr;
	}
	const int _randIndex = UKML::RandomIntegerInRange(0, _roomsWithAvailablesDoors.Num() - 1);

	return _roomsWithAvailablesDoors[_randIndex];
}

void UWorldGeneratorSubsystem::Multi_SetAllActorLocation_Implementation(const FVector& _newLoc)
{
	UWorld* _world = GetWorld();
	if (!_world) return;
	APlayerController* _controller = _world->GetFirstPlayerController();
	if (!_controller) return;
	APawn* _pawn = _controller->GetPawn();
	if (!_pawn) return;
	_pawn->SetActorLocation(_newLoc);
}

void UWorldGeneratorSubsystem::UpdateAllDoors()
{
	for (ADoor* _door : allConnectedDoor)
	{
		if (!IsValid(_door)) continue;
		_door->Destroy();
	}
	for (ADoor* _door : allOccludedDoors)
	{
		if (!IsValid(_door)) continue;
		FActorSpawnParameters _param;
		_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(data->occludedDoor, _door->GetActorLocation(), _door->GetActorRotation(), _param);
		_door->Destroy();
	}
	allOccludedDoors.Empty();
	allConnectedDoor.Empty();
}

FVector UWorldGeneratorSubsystem::GetNewPos()
{
	return allRooms[0]->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
}

void UWorldGeneratorSubsystem::GenerateWorld()
{
	if (!data) return;
	GenerateFirstRoom();
	if (!canGenerate) return;
	GenerateAllRooms();
	OccludeAvailablesDoors();
	UpdateAllDoors();
	FTimerHandle _timer;
	//Multi_SetAllActorLocation(allRooms[0]->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
}