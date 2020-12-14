// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/CollisionProfile.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "WeaponBase.generated.h"


// Forward declarations

class AChaliceCharacter;
class UWeaponTraceComponent;
class UChaliceAbility;
struct FGameplayTag;


/**
 * Base class for weapons, including a weapon trace component, mesh component, and common weapon properties
 */
UCLASS(Abstract)
class CHALICEGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:

	AWeaponBase();


	// Actor overrides


	// Weapon controls

	// Use this instead of attach to component to properly track the character owner
	void Equip(AChaliceCharacter* NewOwningCharacter, USceneComponent* NewParentComponent, const FAttachmentTransformRules& AttachmentRules, FName SocketName = NAME_None);

	// Use this instead of detach from actor to properly track the character owner
    void Unequip(const FDetachmentTransformRules& DetachmentRules);

	UFUNCTION(BlueprintCallable, Category="Weapon", meta=(DisplayName="Equip"))
	void BP_Equip(AChaliceCharacter* NewOwningCharacter, USceneComponent* NewParentComponent,
		EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies,
		FName SocketName = NAME_None);

	UFUNCTION(BlueprintCallable, Category="Weapon", meta=(DisplayName="Unequip"))
	void BP_Unequip(EDetachmentRule LocationRule, EDetachmentRule RotationRule, EDetachmentRule ScaleRule);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool TargetRequirementsMet(const FGameplayTagContainer& TargetTags) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool InterruptRequirementsMet(const FGameplayTagContainer& TargetTags) const;

	
	

	// Accessors

	AChaliceCharacter* GetCharacterOwner() const { return CharacterOwner; }
	UMeshComponent* GetMeshComponent() const { return MeshComponent; }
	UWeaponTraceComponent* GetTraceComponent() const { return TraceComponent; }
	bool IsWeaponActive() const { return bWeaponActive; }

	// Returns all the weapon tags, plus any character owned tags (if this weapon is equipped by a character)
	UFUNCTION(BlueprintCallable, Category="Weapon")
    FGameplayTagContainer GetTags() const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FName GetCollisionProfile() const;
	

	// Component name constants

	static FName MeshComponentName;
	static FName TraceComponentName;


	// Editor properties

	// Abilities granted to a character when this weapon is equipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TArray<TSubclassOf<UChaliceAbility>> Abilities;

	// Tags added to weapon hit events in addition to owning character tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagContainer WeaponTags;

	// Weapon specific target requirements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagQuery TargetRequirements;

	// Weapon specific interrupt requirements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagQuery InterruptRequirements;

	// If this is true, use this weapon's collision profile instead of the character's collision profile.
	// Note: if this weapon isn't owned by a character, this settings won't do anything
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	bool bOverrideCharacterCollisionProfile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FCollisionProfileName CollisionProfile;


protected:

	// Callbacks

	void OnWeaponActivationEvent(const FGameplayTag Tag, int32 Count);
	

	// Events

	virtual void OnWeaponActivated();
	virtual void OnWeaponDeactivated();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon", meta=(DisplayName="On Weapon Activated"))
	void BP_OnWeaponActivated();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon", meta=(DisplayName="On Weapon Deactivated"))
	void BP_OnWeaponDeactivated();


private:

	// Character tracking
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	AChaliceCharacter* CharacterOwner;

	FDelegateHandle ActivationDelegateHandle;
	bool bWeaponActive = false;
	

	// Default components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UWeaponTraceComponent* TraceComponent;
	
};
