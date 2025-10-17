// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerZone.h"
#include "StartGamePlayerZone.generated.h"

// When waiting for players, start the game when every players are in the triggerzone for x time
UCLASS()
class PUMPKING_API AStartGamePlayerZone : public ATriggerZone
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameTimerStarted);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameTimerCanceled);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartGameTimerTicked, int, remainingTimeBeforeStarting);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameCompleted);

private:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (Units="s")) int timeBeforeStarting = 5;

	UPROPERTY(VisibleAnywhere) int currentPlayerCount = 0;
	
	FTimerHandle startGameTimer;
	float fullTimerElapsedTime = 0;
public:
	//When the start game timer starts, meaning the countdown start
	UPROPERTY(BlueprintAssignable)
	FOnStartGameTimerStarted onStartGameTimerStarted;
	//When someone leaves the trigger and the game start is canceled
	UPROPERTY(BlueprintAssignable)
	FOnStartGameTimerCanceled onStartGameTimerCanceled;
	// When the timer ticks
	UPROPERTY(BlueprintAssignable)
	FOnStartGameTimerTicked onStartGameTimerTicked;
	// When the timer runs out of time, and all the playres are ready
	UPROPERTY(BlueprintAssignable)
	FOnStartGameCompleted onStartGameCompleted;

public:	
	AStartGamePlayerZone();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnTriggerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnTriggerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	//when the startGameTimer is called, it's supposed to be the "count down" of the "start game"
	void OnStartGameTimerTicked();
};
