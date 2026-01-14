// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/FlashLightComponent.h"
#include "Macro.h"
#include "Player/PlayerCharacter.h"

// Sets default values for this component's properties
UFlashLightComponent::UFlashLightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFlashLightComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	// ...
	
}

void UFlashLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FlashLightMovement();
	// ...
}

void UFlashLightComponent::Init()
{
	character = Cast<APlayerCharacter>(GetOwner());
	if (!character)
		LOG_ERROR("No Player Character");
}

void UFlashLightComponent::FlashLightMovement()
{
	if (!flashLight || !character)return;
	if (!character->IsLocallyControlled())return;
	FRotator _current = flashLight->GetActorRotation();
	FRotator _target = character->GetController()->GetControlRotation();
	//float _yawDiff = character->GetController()->GetControlRotation() - flashLight->GetActorRotation();
	//FRAME_LOG(FString::SanitizeFloat(_yawDiff));
	//_yawDiff /= 360.0f;
	//FRAME_LOG(FString::SanitizeFloat(_yawDiff)+ "/180");

	float Angle = _current.Quaternion().AngularDistance(_target.Quaternion());
	Angle = FMath::RadiansToDegrees(Angle);
	FRAME_LOG(FString::SanitizeFloat(Angle));
	float _newSpeed = flashLightSpeedRotation + flashLightSpeedRotation * Angle/ 90.0f;
	FRAME_LOG(FString::SanitizeFloat(_newSpeed));
	FRotator _newRot = FMath::RInterpConstantTo(_current, _target,DELTATIME, _newSpeed);
	flashLight->SetActorRotation(_newRot);
	flashLight->SetActorLocation(character->GetActorLocation());

	if (flashLight)
	{
		Server_ReplicateFlashLightPositionAndLocation(flashLight->GetActorLocation(), flashLight->GetActorRotation());
	}
}

void UFlashLightComponent::Server_ReplicateFlashLightPositionAndLocation_Implementation(const FVector& _position, const FRotator& _rotation)
{
	Multi_ReplicateFlashLightPositionAndLocation(_position, _rotation);
}

void UFlashLightComponent::Multi_ReplicateFlashLightPositionAndLocation_Implementation(const FVector& _position, const FRotator& _rotation)
{
	if (!flashLight)return;
	flashLight->SetActorLocation(_position);
	flashLight->SetActorRotation(_rotation);
}

void UFlashLightComponent::Server_SpawnFlashLight_Implementation()
{
	Multi_SpawnFlashLight();
}

void UFlashLightComponent::Server_DespawnFlashLight_Implementation()
{
	Multi_DespawnFlashLight();
}

void UFlashLightComponent::Multi_DespawnFlashLight_Implementation()
{
	//LOG("DespawnFlashLight");
	if (!flashLight)return;
	flashLight->SetLifeSpan(0.1f);
}

void UFlashLightComponent::Multi_SpawnFlashLight_Implementation()
{
	//LOG("SpawnFlashLight");
	FActorSpawnParameters _spawnParam = FActorSpawnParameters();
	_spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	flashLight = WORLD->SpawnActor<ASpotLight>(ASpotLight::StaticClass(), character->GetActorTransform(), FActorSpawnParameters());
	if(flashLight)
		flashLight->SetMobility(EComponentMobility::Movable);
}

void UFlashLightComponent::ActivateFlashLight()
{
	if (useToggle && isActivate)
	{
		isActivate = false;
		Server_DespawnFlashLight();
		return;
	}

	isActivate = true;
	Server_SpawnFlashLight();
}

void UFlashLightComponent::DesactivateFlashLight()
{
	if (useToggle)return;
	isActivate = false;
	Server_DespawnFlashLight();
}

