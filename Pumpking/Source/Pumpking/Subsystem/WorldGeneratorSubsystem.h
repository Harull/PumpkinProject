// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include <DataAssets/WorldGeneratorDataAsset.h>
#include "WorldGeneratorSubsystem.generated.h"

class ARoom;

UCLASS(Blueprintable)
class PUMPKING_API UWorldGeneratorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	//Generaton Related
	UPROPERTY() int generatedRoomCount = 0;
	UPROPERTY() bool canGenerate = true;
	UPROPERTY() TSubclassOf<ARoom> lastPreset = nullptr;
	UPROPERTY() TArray<TObjectPtr<ARoom>> allRooms = TArray<TObjectPtr<ARoom>>();

	//Data
	UPROPERTY(EditAnywhere) TObjectPtr<UWorldGeneratorDataAsset> data = nullptr;

public:
	FORCEINLINE void SetDataAsset(TObjectPtr<UWorldGeneratorDataAsset> _data) { data = _data; }

private:
	TObjectPtr<ARoom> GenerateNewRoom(const FVector& _position);
	bool ComputeNewPosForRoom(const FVector& _currentDoorPos, const FVector& _direction, TObjectPtr<ARoom> _newRoom);
	TSubclassOf<ARoom> GetRandomRoomPreset();
	void GenerateFirstRoom();
	TObjectPtr<ARoom> GetRandomRoomWithAvailableDoor();
public:
	void GenerateWorld();
};
