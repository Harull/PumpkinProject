// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Door.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ARoom::OnConstruction(const FTransform& Transform)
{
	UKismetSystemLibrary::PrintString(this, "OnConstruction Called");
	RetrieveAllDoors();

}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoom::RetrieveAllDoors()
{
	TArray<AActor*> _outActors = {};
	UKismetSystemLibrary::PrintString(this, "RetrieveAllDoors");

	GetAllChildActors(_outActors);
	for (AActor* _actor : _outActors)
	{
		if (TObjectPtr<UChildActorComponent> _childActorCmpnt = Cast<UChildActorComponent>(_actor)) {
			UKismetSystemLibrary::PrintString(this, "Found a CHILD ACTOR COMPONENT int the children of the actor");
		}

		if (TObjectPtr<ADoor> _door = Cast<ADoor>(_actor)) {
			UKismetSystemLibrary::PrintString(this, "Found a DOOR ACTOR in the children of the actor");
			doorsInRoom.Add(_door);
		}
	}
}

