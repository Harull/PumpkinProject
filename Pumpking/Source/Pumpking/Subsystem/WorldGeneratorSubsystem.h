// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include <DataAssets/WorldGeneratorDataAsset.h>
#include "WorldGeneratorSubsystem.generated.h"

class ARoom;
class ADoor;

UCLASS(Blueprintable)
class PUMPKING_API UWorldGeneratorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	//Generaton Related
	UPROPERTY() int generatedRoomCount = 0;
	UPROPERTY() bool canGenerate = true;
	UPROPERTY() TSubclassOf<ARoom> lastPreset = nullptr;
	UPROPERTY() TArray<TObjectPtr<ARoom>> allRooms = TArray<TObjectPtr<ARoom>>();
	UPROPERTY() TArray<TObjectPtr<ADoor>> allConnectedDoor = TArray<TObjectPtr<ADoor>>();
	UPROPERTY() TArray<TObjectPtr<ADoor>> allOccludedDoors = TArray<TObjectPtr<ADoor>>();

	//Data
	UPROPERTY(EditAnywhere) TObjectPtr<UWorldGeneratorDataAsset> data = nullptr;

public:
	FORCEINLINE void SetDataAsset(TObjectPtr<UWorldGeneratorDataAsset> _data) { data = _data; }

private:
	TObjectPtr<ARoom> GenerateNewRoom(const FVector& _position);
	bool ComputeNewPosForRoom(const FVector& _currentDoorPos, const FVector& _direction, TObjectPtr<ARoom> _newRoom);
	TSubclassOf<ARoom> GetRandomRoomPreset();
	void GenerateFirstRoom();
	void GenerateAllRooms();
	void OccludeAvailablesDoors();
	TObjectPtr<ARoom> GetRandomRoomWithAvailableDoor();
	UFUNCTION(NetMulticast, Reliable) void Multi_SetAllActorLocation(const FVector& _newLoc);
	UFUNCTION(NetMulticast, Reliable) void Multi_UpdateAllDoor(const TArray<ADoor*>& _occludedDoor, const TArray<ADoor*>& _connectedDoors, TSubclassOf<AActor> _occuldedDoorPreset);
	void UpdateAllDoors();
public:
	void GenerateWorld();
};
