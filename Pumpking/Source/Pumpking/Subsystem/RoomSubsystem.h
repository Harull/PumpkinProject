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
	UPROPERTY() float maxDist = 4000.0f;

	void Initialize(FSubsystemCollectionBase& _collection) override;
	bool IsRoomRelevantForPlayer(TObjectPtr<ARoom> _room, TObjectPtr<APawn> _pawn) const;
public:
	void UpdateRoomsForAllPlayers(TArray<TObjectPtr<APlayerState>> _players);
	void AddRoom(TObjectPtr<ARoom> _newRoom);
	void RemoveRoom(TObjectPtr<ARoom> _newRoom);
};
