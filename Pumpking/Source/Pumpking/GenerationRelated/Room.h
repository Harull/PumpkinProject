// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class ADoor;

UCLASS()
class PUMPKING_API ARoom : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere) FBox roomBox;
	UPROPERTY(EditAnywhere) bool isEnable = true;
	UPROPERTY(VisibleAnywhere) TArray<TObjectPtr<ADoor>> doorsInRoom = {};
	UPROPERTY(VisibleAnywhere) bool hasAvailableDoor = true;
	UPROPERTY(VisibleAnywhere) TArray<TObjectPtr<UStaticMeshComponent>> allMeshes = {};
private:

public:
	ARoom();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable) int GetDoorsCount() const { return doorsInRoom.Num(); }
	FORCEINLINE TArray<TObjectPtr<ADoor>> GetAllDoors() const { return doorsInRoom; }
	FORCEINLINE bool HasAvailableDoor() const { return hasAvailableDoor; }
	FORCEINLINE FBox GetRoomCollision() const { return roomBox; }

	UFUNCTION(NetMulticast, Reliable) void Multi_UpdateRoomState(ARoom* _room, bool _newState);

public:
	/// <summary>
	/// Retrieve in the children of this actor the child actor components, and check if it's a door, if it is, add it to the doorsInRoom array
	/// </summary>
	UFUNCTION(BlueprintCallable) void RetrieveAllDoors();
	/// <summary>
	/// Compute the distance of the room with a door
	/// </summary>
	float GetDistanceWithDoor(TObjectPtr<ADoor> _door);
	float GetDistanceWithDoor(const int _index);
	void ComputeCollision();
	void RemoveDoor(TObjectPtr<ADoor> _door);

	void ChangeRoomState(const bool _enabled);

	TObjectPtr<ADoor> GetFirstAvailableDoor();
	TObjectPtr<ADoor> GetRandomAvailableDoor();
	TArray< TObjectPtr<ADoor>> GetAllAvailablesDoors();
};
