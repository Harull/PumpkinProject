// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "Door.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Macro.h>
#include <Subsystem/RoomSubsystem.h>

// Sets default values
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ARoom::OnConstruction(const FTransform& Transform)
{
	//UKismetSystemLibrary::PrintString(this, "OnConstruction Called");
	RetrieveAllDoors();
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<URoomSubsystem>()->AddRoom(this);
}

void ARoom::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetSubsystem<URoomSubsystem>()->RemoveRoom(this);
}

void ARoom::Multi_UpdateRoomState_Implementation(ARoom* _room, bool _newState)
{
	const int _size = allMeshes.Num();
	for (int _index = 0; _index < _size; _index++)
	{
		UStaticMeshComponent* _mesh = _room->allMeshes[_index];
		if (!_mesh) continue;

		_mesh->SetCollisionEnabled(_newState ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		_mesh->SetHiddenInGame(!_newState);
	}
	_room->isEnable = _newState;
}

void ARoom::RetrieveAllDoors()
{
	doorsInRoom.Empty();
	TArray<AActor*> _outActors = {};
	//UKismetSystemLibrary::PrintString(this, "RetrieveAllDoors");

	GetAllChildActors(_outActors);
	for (AActor* _actor : _outActors)
	{
		if (TObjectPtr<ADoor> _door = Cast<ADoor>(_actor)) {
			doorsInRoom.Add(_door);
		}
	}
}

float ARoom::GetDistanceWithDoor(TObjectPtr<ADoor> _door)
{
	return FVector::Dist(GetActorLocation(), _door->GetActorLocation());
}

float ARoom::GetDistanceWithDoor(const int _index)
{
	bool _validIndex = _index >= 0 && _index < doorsInRoom.Num() - 1;
	return _validIndex ? GetDistanceWithDoor(doorsInRoom[_index]) : INFINITY;
}

void ARoom::ComputeCollision()
{
	TArray<UStaticMeshComponent*> _allMeshes;
	GetComponents<UStaticMeshComponent*>(_allMeshes);
	for (UStaticMeshComponent* _mesh : _allMeshes)
	{
		if (_mesh)
		{
			roomBox += _mesh->Bounds.GetBox();
		}
	}
	roomBox = roomBox.BuildAABB(roomBox.GetCenter(), roomBox.GetExtent() - FVector(5.0f));

	if (roomBox.IsValid)
	{
		DrawDebugBox(GetWorld(), roomBox.GetCenter(), roomBox.GetExtent(), FColor::Red, false, 1.0f, 0, 5.0f);
	}
	allMeshes = _allMeshes;
}

void ARoom::RemoveDoor(TObjectPtr<ADoor> _door)
{
	doorsInRoom.Remove(_door);
	_door->Destroy();
}

void ARoom::ChangeRoomState(const bool _enabled)
{
	if (allMeshes.IsEmpty()) return;
	if (isEnable == _enabled) return;
	Multi_UpdateRoomState(this, _enabled);
	const int _size = allMeshes.Num();
	for (int _index = 0; _index < _size; _index++)
	{
		UStaticMeshComponent* _mesh = allMeshes[_index];
		if (!_mesh) continue;

		_mesh->SetCollisionEnabled(_enabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		_mesh->SetHiddenInGame(!_enabled);
	}
	isEnable = _enabled;
}

TObjectPtr<ADoor> ARoom::GetFirstAvailableDoor()
{
	for (TObjectPtr<ADoor> _door : doorsInRoom)
	{
		if (_door->GetIsAvailable())
			return _door;
	}
	return nullptr;
}

TObjectPtr<ADoor> ARoom::GetRandomAvailableDoor()
{
	TArray<TObjectPtr<ADoor>> _allAvailableDoors;
	for (TObjectPtr<ADoor> _door : doorsInRoom)
	{
		if (_door && _door->GetIsAvailable())
		{
			_allAvailableDoors.Add(_door);
		}
	}
	if (_allAvailableDoors.IsEmpty())
	{
		hasAvailableDoor = false;
		return nullptr;
	}
	const int _randIndex = UKML::RandomIntegerInRange(0, _allAvailableDoors.Num() - 1);
	return _allAvailableDoors[_randIndex];
}

TArray<TObjectPtr<ADoor>> ARoom::GetAllAvailablesDoors()
{
	TArray<TObjectPtr<ADoor>> _allAvailableDoors;
	for (TObjectPtr<ADoor> _door : doorsInRoom)
	{
		if (_door && _door->GetIsAvailable())
		{
			_allAvailableDoors.Add(_door);
		}
	}

	return _allAvailableDoors;
}