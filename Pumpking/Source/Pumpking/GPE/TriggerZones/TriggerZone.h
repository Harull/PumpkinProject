// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "TriggerZone.generated.h"

UCLASS()
class PUMPKING_API ATriggerZone : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) TObjectPtr<UBoxComponent> triggerZone;

public:	
	// Sets default values for this actor's properties
	ATriggerZone();

protected:
	virtual void BeginPlay() override;

private:
	void Init();
	void InitEvents();

protected:
	UFUNCTION()
	virtual void OnTriggerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnTriggerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
