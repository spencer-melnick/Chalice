// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"


// AChaliceCharacter

AChaliceCharacter::AChaliceCharacter()
{
	// Enable tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create default components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(SpringArmComponentName);
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(CameraComponentName);
	CameraComponent->SetupAttachment(SpringArmComponent);
}


// Component name constants

FName AChaliceCharacter::SpringArmComponentName(TEXT("SpringArmComponent"));
FName AChaliceCharacter::CameraComponentName(TEXT("CameraComponent"));


// Character overrides

void AChaliceCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AChaliceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	// Bind jump controls
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AChaliceCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AChaliceCharacter::StopJumping);

	// Bind movement controls
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AChaliceCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AChaliceCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AChaliceCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AChaliceCharacter::AddControllerYawInput);
}

void AChaliceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



// Movement controls

void AChaliceCharacter::AddMovementInputControlSpace(FVector ControlSpaceInput, float Scale, bool bForce)
{
	const FRotator ControlRotation = GetControlRotation();
	const FRotator ControlYaw(0.f, ControlRotation.Yaw, 0.f);
	AddMovementInput(ControlYaw.RotateVector(ControlSpaceInput), Scale, bForce);
}

void AChaliceCharacter::MoveForward(float Scale)
{
	AddMovementInputControlSpace(FVector::ForwardVector, Scale, false);
}

void AChaliceCharacter::MoveRight(float Scale)
{
	AddMovementInputControlSpace(FVector::RightVector, Scale, false);
}

