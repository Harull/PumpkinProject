// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/SpotLight.h"
#include "FlashLightComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUMPKING_API UFlashLightComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) TObjectPtr<APlayerCharacter> character = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<ASpotLight> flashLight = nullptr;
	UPROPERTY(EditAnywhere) bool isActivate = false;

public:	
	// Sets default values for this component's properties
	UFlashLightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();
	void FlashLightMovement();

	UFUNCTION(Server, Reliable) void Server_SpawnFlashLight();
	UFUNCTION(Server, Reliable) void Server_DespawnFlashLight();
	UFUNCTION(NetMulticast, Reliable) void Multi_DespawnFlashLight();
	UFUNCTION(NetMulticast, Reliable) void Multi_SpawnFlashLight();
	UFUNCTION(Server, Reliable) void Server_ReplicateFlashLightPositionAndLocation(const FVector& _position, const FRotator& _rotation);
	UFUNCTION(NetMulticast, Reliable) void Multi_ReplicateFlashLightPositionAndLocation(const FVector& _position, const FRotator& _rotation);



public:
	void ActivateFlashLight();
	void DesactivateFlashLight();

	
};
