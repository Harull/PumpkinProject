// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGamePlayerZone.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"

// Sets default values
AStartGamePlayerZone::AStartGamePlayerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartGamePlayerZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStartGamePlayerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AStartGamePlayerZone::OnTriggerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())return;
	if (!Cast<ACharacter>(OtherActor))return;

	currentPlayerCount++;

	//If the number of players inside the triggerzone, are all the players of the server
	if (currentPlayerCount == GetWorld()->GetAuthGameMode()->GetNumPlayers())
	{
		GetWorld()->GetTimerManager().SetTimer(startGameTimer, this, &AStartGamePlayerZone::OnStartGameTimerTicked, 1, true);
		fullTimerElapsedTime = 0;
		onStartGameTimerStarted.Broadcast();
	}
}

void AStartGamePlayerZone::OnTriggerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority())return;
	if (!Cast<ACharacter>(OtherActor))return;

	currentPlayerCount--;

	//If a player left the zone when all the player were inside
	if (currentPlayerCount == GetWorld()->GetAuthGameMode()->GetNumPlayers() - 1)
	{
		onStartGameTimerCanceled.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(startGameTimer);
	}
}

void AStartGamePlayerZone::OnStartGameTimerTicked()
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	fullTimerElapsedTime += _timerManager.GetTimerElapsed(startGameTimer);
	const int _elapsedTimeInSeconds = static_cast<int>(fullTimerElapsedTime);

	onStartGameTimerTicked.Broadcast(timeBeforeStarting - fullTimerElapsedTime + 1);

	// If this is the last tick of the timer
	if (timeBeforeStarting <= fullTimerElapsedTime)
	{
		onStartGameCompleted.Broadcast();
		_timerManager.ClearTimer(startGameTimer);
	}
}

