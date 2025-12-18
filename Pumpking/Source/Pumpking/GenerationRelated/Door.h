// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PUMPKING_API ADoor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere) TObjectPtr<class UArrowComponent> rootArrow;

private:
	//usefull data for generation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess)) bool bIsAvailable = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess)) bool bIsConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess)) bool bIsCondamned = false;

public:	
	ADoor();

public:
	FORCEINLINE void SetIsAvailable(const bool _bValue) {
		bIsAvailable = _bValue;
	}
	FORCEINLINE void SetIsConnected(const bool _bValue) {
		bIsConnected = _bValue;
		bIsAvailable = bIsConnected ? true : bIsAvailable;
	}
	FORCEINLINE void SetIsCondamned(const bool _bValue) {
		bIsCondamned = _bValue;
		bIsAvailable = bIsCondamned ? true : bIsAvailable;
	}
	FORCEINLINE bool GetIsAvailable()const { return bIsAvailable; }
	FORCEINLINE bool GetIsConnected()const { return bIsConnected; }
	FORCEINLINE bool GetIsCondamned()const { return bIsCondamned; }


};
