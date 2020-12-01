// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "WeaponBase.generated.h"


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
 * Base class for weapons held by characters. Grants abilities associated with that weapon, and performs traces for
 * collision. Can be physical weapons like swords, or abstract weapons like hit boxes on a character's fists
 */
UCLASS()
class CHALICEGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:

	AWeaponBase();


	// Component name constants

	static FName MeshComponentName;


	// Actor overrides

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;


	// Weapon controls

	// Starts tracing weapon colliders on every tick, generating hit events for relevant ability components
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EnableTrace();

	// Stops weapon tracing
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DisableTrace();


	// Accessors

	AChaliceCharacter* GetWeaponOwner() const;
	UMeshComponent* GetMeshComponent() const { return MeshComponent; }
	bool IsTraceEnabled() const { return bTraceEnabled; }


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTagContainer WeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName TraceProfile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
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

	// Default components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UMeshComponent* MeshComponent;


	// Weapon properties

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	bool bTraceEnabled = false;


	// Editor display

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	void UpdateEditorSpheres();

	UPROPERTY(Transient)
	TArray<USphereComponent*> EditorSpheres;
#endif
	
};
