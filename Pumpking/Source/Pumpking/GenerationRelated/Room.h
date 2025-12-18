// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"



UCLASS()
class PUMPKING_API ARoom : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere) TArray<TObjectPtr<class ADoor>> doorsInRoom = {};
private:


public:	
	ARoom();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable) int GetDoorsCount() const { return doorsInRoom.Num(); }

public:
	/// <summary>
	/// Retrieve in the children of this actor the child actor components, and check if it's a door, if it is, add it to the doorsInRoom array
	/// </summary>
	UFUNCTION(BlueprintCallable) void RetrieveAllDoors();
};
