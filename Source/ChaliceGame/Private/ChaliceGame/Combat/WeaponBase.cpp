// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Combat/WeaponBase.h"
#include "ChaliceGame/Combat/WeaponTraceComponent.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceGame.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitySetttings.h"


// AWeaponBase

AWeaponBase::AWeaponBase()
{
	// Disable ticking on weapons
	PrimaryActorTick.bCanEverTick = false;

	// Create default components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(MeshComponentName);
	RootComponent = MeshComponent;
	TraceComponent = CreateDefaultSubobject<UWeaponTraceComponent>(TraceComponentName);
	TraceComponent->SetupAttachment(RootComponent);
}


// Component name constants

FName AWeaponBase::MeshComponentName(TEXT("MeshComponent"));
FName AWeaponBase::TraceComponentName(TEXT("TraceComponent"));


// Weapon controls

void AWeaponBase::Equip(AChaliceCharacter* NewOwningCharacter, USceneComponent* NewParentComponent, const FAttachmentTransformRules& AttachmentRules, FName SocketName)
{
	if (!NewOwningCharacter || !NewParentComponent)
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed on %s - invalid parent"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	// Attach to the component
	if (CharacterOwner && CharacterOwner != NewOwningCharacter)
	{
		Unequip(FDetachmentTransformRules(AttachmentRules, true));
	}
	CharacterOwner = NewOwningCharacter;
	AttachToComponent(NewParentComponent, AttachmentRules, SocketName);

	// Track weapon activation
	const FGameplayTag& WeaponActivationTag = UChaliceAbilitySettings::Get()->WeaponActivationTag;
	UChaliceAbilityComponent* AbilityComponent = CharacterOwner->GetChaliceAbilityComponent();
	ActivationDelegateHandle = AbilityComponent->RegisterGameplayTagEvent(WeaponActivationTag)
		.AddUObject(this, &AWeaponBase::OnWeaponActivationEvent);

	// Trigger weapon activation if the tag is already present
	if (AbilityComponent->HasMatchingGameplayTag(WeaponActivationTag))
	{
		OnWeaponActivated();
	}
}

void AWeaponBase::Unequip(const FDetachmentTransformRules& DetachmentRules)
{
	DetachFromActor(DetachmentRules);

	if (!CharacterOwner)
	{
		return;
	}
	
	// Stop tracking weapon activation
	const FGameplayTag& WeaponActivationTag = UChaliceAbilitySettings::Get()->WeaponActivationTag;
	UChaliceAbilityComponent* AbilityComponent = CharacterOwner->GetChaliceAbilityComponent();
	AbilityComponent->UnregisterGameplayTagEvent(ActivationDelegateHandle, WeaponActivationTag);
	CharacterOwner = nullptr;

	// Trigger weapon deactivation if it was removed while active
	if (bWeaponActive)
	{
		bWeaponActive = false;
		OnWeaponDeactivated();
	}
}

void AWeaponBase::BP_Equip(AChaliceCharacter* NewOwningCharacter, USceneComponent* NewParentComponent, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies, FName SocketName)
{
	Equip(NewOwningCharacter, NewParentComponent, FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies), SocketName);
}

void AWeaponBase::BP_Unequip(EDetachmentRule LocationRule, EDetachmentRule RotationRule, EDetachmentRule ScaleRule)
{
	Unequip(FDetachmentTransformRules(LocationRule, RotationRule, ScaleRule, true));
}


// Callbacks

void AWeaponBase::OnWeaponActivationEvent(const FGameplayTag Tag, int32 Count)
{
	if (Count == 0 && bWeaponActive)
	{
		bWeaponActive = false;
		OnWeaponDeactivated();
	}
	else if (Count != 0 && !bWeaponActive)
	{
		bWeaponActive = true;
		OnWeaponActivated();
	}
}


// Events

void AWeaponBase::OnWeaponActivated()
{
	TraceComponent->EnableTrace();

	BP_OnWeaponActivated();
}

void AWeaponBase::OnWeaponDeactivated()
{
	TraceComponent->DisableTrace();

	BP_OnWeaponDeactivated();
}

