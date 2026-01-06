// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerationRelated/Door.h"
#include <Components/ArrowComponent.h>

// Sets default valuesou
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	RootComponent = rootArrow = CreateDefaultSubobject<UArrowComponent>("root");
}