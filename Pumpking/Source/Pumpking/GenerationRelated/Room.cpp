// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "Door.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Macro.h>

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
}

void ARoom::RetrieveAllDoors()
{
	doorsInRoom.Empty();
	TArray<AActor*> _outActors = {};
	//UKismetSystemLibrary::PrintString(this, "RetrieveAllDoors");

	GetAllChildActors(_outActors);
	for (AActor* _actor : _outActors)
	{
		if (TObjectPtr<UChildActorComponent> _childActorCmpnt = Cast<UChildActorComponent>(_actor)) {
			//UKismetSystemLibrary::PrintString(this, "Found a CHILD ACTOR COMPONENT int the children of the actor");
		}

		if (TObjectPtr<ADoor> _door = Cast<ADoor>(_actor)) {
			//UKismetSystemLibrary::PrintString(this, "Found a DOOR ACTOR in the children of the actor");
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
	for (UStaticMeshComponent* mesh : _allMeshes)
	{
		if (mesh)
		{
			roomBox += mesh->Bounds.GetBox();
		}
	}
	roomBox = roomBox.BuildAABB(roomBox.GetCenter(), roomBox.GetExtent() - FVector(15.0f));

	if (roomBox.IsValid)
	{
		DrawDebugBox(GetWorld(), roomBox.GetCenter(), roomBox.GetExtent(), FColor::Red, false, 1.0f, 0, 5.0f);
	}
}

void ARoom::RemoveDoor(TObjectPtr<ADoor> _door)
{
	doorsInRoom.Remove(_door);
	_door->Destroy();
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