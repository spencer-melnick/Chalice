// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "WeaponTraceComponent.generated.h"


// Forward declarations

class AChaliceCharacter;
class USphereComponent;


/**
 * Data type that contains information needed for weapon tracing
 */
USTRUCT(BlueprintType)
struct FWeaponTraceShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Socket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadOnly)
	FVector LastPosition;
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

	AChaliceCharacter* GetWeaponOwner() const;
	bool IsTraceEnabled() const { return bTraceEnabled; }


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagContainer WeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName TraceProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TArray<FWeaponTraceShape> TraceShapes;

	// Only trigger hit events for targets with all of the following tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagRequirements TargetRequirements;

	// If any object hit meets these requirements, ignore all other collisions (useful for shields)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagRequirements InterruptRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTag HitEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTag InterruptEventTag;

protected:

	// Trace functions

	FTransform GetTraceShapeTransform(const FWeaponTraceShape& TraceShape) const;
	FVector GetTraceShapeLocation(const FWeaponTraceShape& TraceShape) const;
	void UpdateTraceLocations();
	FGameplayEventData CreateEventFromTrace(const FHitResult& HitResult, const FWeaponTraceShape& Shape, const FGameplayTag EventTag) const;
	virtual void TraceWeapon();

	// Checks to see if the object hit during the trace meets target requirements
	virtual bool MeetsTargetRequirements(const FHitResult& HitResult);

	// Checks to see if the object hit during the trace meets interruption requirements
	virtual bool MeetsInterruptRequirements(const FHitResult& HitResult);


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
