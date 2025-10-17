// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZone.h"

// Sets default values
ATriggerZone::ATriggerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	triggerZone = CreateDefaultSubobject<UBoxComponent>("Triggerzone");

	triggerZone->SetupAttachment(RootComponent);
}


void ATriggerZone::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ATriggerZone::Init()
{
	InitEvents();
}

void ATriggerZone::InitEvents()
{
	triggerZone->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZone::OnTriggerEntered);
	triggerZone->OnComponentEndOverlap.AddDynamic(this, &ATriggerZone::OnTriggerLeft);
}

void ATriggerZone::OnTriggerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATriggerZone::OnTriggerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

