// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGeneratorSubsystem.h"
#include "Pumpking/GenerationRelated/Room.h"
#include "Pumpking/GenerationRelated/Door.h"
#include "Engine/OverlapResult.h"
#include <Macro.h>

TObjectPtr<ARoom> UWorldGeneratorSubsystem::GenerateNewRoom(const FVector& _position)
{
	TSubclassOf<ARoom> _roomType = GetRandomRoomPreset();
	if (lastPreset == nullptr)
		lastPreset = _roomType;
	if (data->allRoomsTypes.Num() > 1)
	{
		while (_roomType == lastPreset)
		{
			_roomType = GetRandomRoomPreset();
		}
	}
	lastPreset = _roomType;
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

	_newRoom->SetActorLocation(_currentDoorPos + _direction * _newRoom->GetDistanceWithDoor(_door));

	_newRoom->ComputeCollision();
	//bool _shouldBeDeleted = false;
	//for (ARoom* _currentRoom : allRooms)
	//{
	//	if (_newRoom == _currentRoom)
	//		continue;

	//	if (_newRoom->GetActorLocation().Equals(_currentRoom->GetActorLocation(), 0.0001))
	//	{
	//		LOG(_newRoom->GetName() + " Destroyed");
	//		_shouldBeDeleted = true;
	//	}
	//}
	//if (_shouldBeDeleted)
	//{
	//	//allRooms.Remove(_newRoom);
	//	_newRoom->Destroy();
	//	return false;
	//}

	_newRoom->RemoveDoor(_door);
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
	generatedRoomCount++;
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

void UWorldGeneratorSubsystem::GenerateWorld()
{
	if (!data) return;
	GenerateFirstRoom();
	if (!canGenerate) return;

	while (canGenerate && (generatedRoomCount < data->maximumRoomCount))
	{
		TObjectPtr<ARoom> _room = GetRandomRoomWithAvailableDoor();
		if (!_room) break;

		TObjectPtr<ADoor> _door = _room->GetRandomAvailableDoor();
		if (!_door) continue;

		TObjectPtr<ARoom> _newRoom = GenerateNewRoom(_door->GetActorLocation());
		if (!_newRoom) continue;

		//_door->SetIsConnected(true);
		bool _isValid = ComputeNewPosForRoom(_door->GetActorLocation(), _door->GetActorForwardVector(), _newRoom);
		if (!_isValid)
			continue;

		_room->RemoveDoor(_door);

		generatedRoomCount++;
	}
}