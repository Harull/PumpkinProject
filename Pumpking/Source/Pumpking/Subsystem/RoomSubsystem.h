// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RoomSubsystem.generated.h"

class ARoom;
UCLASS()
class PUMPKING_API URoomSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	UPROPERTY() TArray<TObjectPtr<ARoom>> allRooms = TArray<TObjectPtr<ARoom>>();
	UPROPERTY() float maxDist = 5000.0f;

	void Initialize(FSubsystemCollectionBase& _collection) override;
	void UpdatelAllRooms();
public:
	void AddRoom(TObjectPtr<ARoom> _newRoom);
	void RemoveRoom(TObjectPtr<ARoom> _newRoom);
};
