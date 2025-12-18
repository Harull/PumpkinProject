// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldGeneratorDataAsset.generated.h"

/**
 *
 */
UCLASS()
class PUMPKING_API UWorldGeneratorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) int maximumRoomCount = 10;
	UPROPERTY(EditAnywhere) int maximumTryPerRoom = 10;
	UPROPERTY(EditAnywhere) TArray<TSubclassOf<AActor>> allRoomsTypes;
};
