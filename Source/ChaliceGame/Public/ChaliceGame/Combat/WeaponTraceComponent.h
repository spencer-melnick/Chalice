// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Engine/CollisionProfile.h"
#include "WeaponTraceComponent.generated.h"


// Forward declarations

class AChaliceCharacter;
class AWeaponBase;
class USphereComponent;


/**
 * Data type that contains information needed for weapon tracing
 */
USTRUCT(BlueprintType)
struct FWeaponTraceShape
{
	GENERATED_BODY()

	// Tags added to any events involving this shape
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ShapeTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	// World location last updated during weapon trace
	UPROPERTY(BlueprintReadOnly)
	FVector CurrentTraceLocation;

	// World location used as start of trace
	UPROPERTY(BlueprintReadOnly)
	FVector PreviousTraceLocation;
};


/**
 * Component for weapon that performs hit traces and generates Blueprint events
 */
UCLASS(Category="Chalice|Combat", meta=(BlueprintSpawnableComponent))
class CHALICEGAME_API UWeaponTraceComponent : public USceneComponent
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FWeaponTraceVisualizer;
#endif

public:

	UWeaponTraceComponent();


	// Component overrides

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Trace controls

	// Starts tracing weapon colliders on every tick, generating hit events for relevant ability components
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EnableTrace();

	// Stops weapon tracing
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DisableTrace();


	// Accessors

	AWeaponBase* GetWeaponActor() const;
	AChaliceCharacter* GetWeaponOwner() const;
	bool IsTraceEnabled() const { return bTraceEnabled; }
	
	// Returns tags owned by this trace component, in addition to weapon owned tags
	UFUNCTION(BlueprintCallable, Category="Weapon")
    FGameplayTagContainer GetTags() const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FName GetCollisionProfile() const;


	// Editor properties

	// Tags added to this weapon trace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagContainer TraceTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TArray<FWeaponTraceShape> TraceShapes;

	// Requirements added to the weapon target requirements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagRequirements TargetRequirements;

	// Requirements added to the weapon interrupt requirements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagRequirements InterruptRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTag InterruptEventTag;

	// If this is true, use this trace component's collision profile instead of the weapon's collision profile.
	// Note: if this trace component isn't owned by a weapon, this setting does nothing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	bool bOverrideWeaponCollisionProfile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FCollisionProfileName CollisionProfile;


	// Trace functions

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FTransform GetTraceShapeTransform(const FWeaponTraceShape& TraceShape) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FVector GetTraceShapeLocation(const FWeaponTraceShape& TraceShape) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FGameplayTagContainer GetTraceShapeTags(const FWeaponTraceShape& TraceShape) const;


	// Extra debug data

#ifdef CHALICE_DEBUG
	TArray<FVector> LastHitLocations;
#endif
	

protected:

	// Trace functions

	void UpdateTraceLocations();
	FGameplayEventData CreateEventFromTrace(const FHitResult& HitResult, const FWeaponTraceShape& Shape, const FGameplayTag EventTag) const;
	virtual void TraceWeapon();

	// Checks to see if the object hit during the trace meets target requirements
	virtual bool TargetRequirementsMet(const FHitResult& HitResult) const;

	// Checks to see if the object hit during the trace meets interruption requirements
	virtual bool InterruptRequirementsMet(const FHitResult& HitResult) const;


	// Events

	virtual void OnTraceHit(TArray<FGameplayEventData>& Events);
	virtual void OnTraceInterrupt(FGameplayEventData Event);

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon", meta=(DisplayName="On Trace Hit"))
	void BP_OnTraceHit(TArray<FGameplayEventData>& Events);

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon", meta=(DisplayName="On Trace Interrupt"))
	void BP_OnTraceInterrupt(FGameplayEventData Event);
	
	
private:

	// Weapon properties

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	bool bTraceEnabled = false;
	
};
