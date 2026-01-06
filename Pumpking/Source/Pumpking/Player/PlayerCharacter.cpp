#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "Camera/CameraComponent.h"
#include "Macro.h"
#include <Subsystem/WorldGeneratorSubsystem.h>

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	meshFps = CreateDefaultSubobject<USkeletalMeshComponent>("MeshFps");
	sprintArm = CreateDefaultSubobject<USpringArmComponent>("SprintArm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	meshFps->SetupAttachment(GetMesh());
	sprintArm->SetupAttachment(GetMesh(), FName("head"));
	camera->SetupAttachment(sprintArm);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	JumpMaxCount = 15;
	if (SERVER)
		LOG("Server");
	else
		LOG("Client");
	if (ULocalPlayer* _localPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* _inputSystem = _localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			_inputSystem->AddMappingContext(mapping, 0.0f);
			LOG("AddMappingContext");
		}
	}

	if ((SERVER && SELF) && wantToGenerate)
	{
		TObjectPtr<UWorldGeneratorSubsystem> _sub = GetWorld()->GetSubsystem<UWorldGeneratorSubsystem>();
		_sub->SetDataAsset(worldGenDataAsset);
		_sub->GenerateWorld();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SERVER)
		Multi_ReplicatePosition(GetActorLocation(), GetActorRotation());
	else
		Server_ReplicatePosition(GetActorLocation(), GetActorRotation());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	_input->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Movement);
	_input->BindAction(moveAction, ETriggerEvent::Completed, this, &APlayerCharacter::Movement);
	_input->BindAction(jumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jumping);
	_input->BindAction(rotateAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Rotate);
}

void APlayerCharacter::Movement(const FInputActionValue& _value)
{
	const FVector2D& _movementInput = _value.Get<FVector2D>();
	inputValue = _movementInput;
	const FRotator& _rotation = GetControlRotation();

	const FVector& _move = GetActorForwardVector() * _movementInput.X + GetActorRightVector() * _movementInput.Y;

	AddMovementInput(_move);
	if (SERVER)
		Multi_ReplicateAnim(inputValue);
	else
		Server_ReplicateAnim(inputValue);
}

void APlayerCharacter::Rotate(const FInputActionValue& _value)
{
	const FVector2D& _rotateInput = _value.Get<FVector2D>();

	AddControllerYawInput(_rotateInput.X);
	AddControllerPitchInput(-_rotateInput.Y);
}

void APlayerCharacter::Jumping()
{
	Jump();
}

void APlayerCharacter::Server_ReplicatePosition_Implementation(const FVector& _position, const FRotator& _rotation)
{
	Multi_ReplicatePosition(_position, _rotation);
}

void APlayerCharacter::Multi_ReplicatePosition_Implementation(const FVector& _position, const FRotator& _rotation)
{
	if (SELF)
		return;
	SetActorLocation(_position);
	SetActorRotation(_rotation);
}

void APlayerCharacter::Server_ReplicateAnim_Implementation(const FVector2D& _inputValue)
{
	Multi_ReplicateAnim(_inputValue);
}

void APlayerCharacter::Multi_ReplicateAnim_Implementation(const FVector2D& _inputValue)
{
	if (SELF)
		return;
	inputValue = _inputValue;
}