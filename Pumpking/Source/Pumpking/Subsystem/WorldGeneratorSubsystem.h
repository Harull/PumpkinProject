// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include <DataAssets/WorldGeneratorDataAsset.h>
#include "WorldGeneratorSubsystem.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class PUMPKING_API UWorldGeneratorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) TObjectPtr<UWorldGeneratorDataAsset> data = nullptr;

public:
	FORCEINLINE void SetDataAsset(TObjectPtr<UWorldGeneratorDataAsset> _data) { data = _data; }

private:
	void GenerateNewRoom();
	void ComputeNewPosForRoom();

public:
	void GenerateWorld();
};
