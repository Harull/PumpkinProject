// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pumpking/DataAssets/WorldGeneratorDataAsset.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class PUMPKING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TObjectPtr<USkeletalMeshComponent> meshFps = nullptr;

	//Camera
	UPROPERTY(EditAnywhere) TObjectPtr<USpringArmComponent> sprintArm = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UCameraComponent> camera = nullptr;

	//Input
	UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> mapping = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> moveAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> jumpAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> rotateAction = nullptr;
	UPROPERTY(EditAnywhere) bool wantToGenerate = false;

	//Anim
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess)) FVector2D inputValue = FVector2D::ZeroVector;

	//Data
	UPROPERTY(EditAnywhere) TObjectPtr<UWorldGeneratorDataAsset> worldGenDataAsset = nullptr;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Movement(const FInputActionValue& _value);
	void Rotate(const FInputActionValue& _value);
	void Jumping();

	UFUNCTION(Server, Reliable) void Server_ReplicatePosition(const FVector& _position, const FRotator& _rotation);
	UFUNCTION(NetMulticast, Reliable) void Multi_ReplicatePosition(const FVector& _position, const FRotator& _rotation);
	UFUNCTION(Server, Reliable) void Server_ReplicateAnim(const FVector2D& _inputValue);
	UFUNCTION(NetMulticast, Reliable) void Multi_ReplicateAnim(const FVector2D& _inputValue);
};
