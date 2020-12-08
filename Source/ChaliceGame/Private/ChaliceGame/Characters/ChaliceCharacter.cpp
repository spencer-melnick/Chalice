// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceGame/Characters/InputBindings.h"
#include "ChaliceGame.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/Abilities/ChaliceAbility.h"
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
	AbilityComponent = CreateDefaultSubobject<UChaliceAbilityComponent>(AbilityComponentName);
}


// Component name constants

FName AChaliceCharacter::SpringArmComponentName(TEXT("SpringArmComponent"));
FName AChaliceCharacter::CameraComponentName(TEXT("CameraComponent"));
FName AChaliceCharacter::AbilityComponentName(TEXT("AbilitySystemComponent"));


// Character overrides

void AChaliceCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GrantStartingAbilities();
	}
}

void AChaliceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	// Bind movement controls
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AChaliceCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AChaliceCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AChaliceCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AChaliceCharacter::AddControllerYawInput);

	// Bind ability activation
	AbilityComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,FGameplayAbilityInputBinds(
		TEXT("Confirm"), TEXT("Cancel"), TEXT("EAbilityInputID"),
		static_cast<uint32>(EAbilityInputID::Confirm), static_cast<uint32>(EAbilityInputID::Cancel)));
}

void AChaliceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AChaliceCharacter::Falling()
{
	Super::Falling();
	
	AbilityComponent->RemoveLooseGameplayTagFully(GroundedTag);
}

void AChaliceCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	AbilityComponent->AddLooseGameplayTagUnique(GroundedTag);
}


// Ability interfaces

UAbilitySystemComponent* AChaliceCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilityComponent);
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


// Combat controls

bool AChaliceCharacter::TargetRequirementsMet(const FGameplayTagContainer& TargetTags) const
{
	return TargetRequirements.RequirementsMet(TargetTags);
}

bool AChaliceCharacter::InterruptRequirementsMet(const FGameplayTagContainer& TargetTags) const
{
	if (InterruptRequirements.IsEmpty())
	{
		// Don't allow interrupts if we don't have any requirements, otherwise every hit would be an interrupt!
		return false;
	}
	return InterruptRequirements.RequirementsMet(TargetTags);	
}


// Ability helper functions

void AChaliceCharacter::GrantStartingAbilities()
{
	if (bGrantedStartingAbilities)
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed: %s already granted starting abilities"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	if (!HasAuthority())
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed: %s does not have authority to grant abilities"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	bGrantedStartingAbilities = true;

	for (const FStartingAbilityInfo& AbilityInfo : StartingAbilities)
	{
		AbilityComponent->GiveAbility(FGameplayAbilitySpec(
			AbilityInfo.AbilityClass, AbilityInfo.AbilityLevel, static_cast<int32>(AbilityInfo.InputBinding)));
	}

	UE_LOG(LogChaliceGame, Display, TEXT("Granted %s starting abilities"),
		*GetNameSafe(this))
}

