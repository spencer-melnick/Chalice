// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceGame/Characters/InputBindings.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTags.h"
#include "Engine/CollisionProfile.h"
#include "ChaliceCharacter.generated.h"


// Forward declarations

class USpringArmComponent;
class UCameraComponent;
class UChaliceAbilityComponent;
class UChaliceAbility;


/**
 * Common information for starting abilities in editor
 */
USTRUCT(BlueprintType)
struct FStartingAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityInputID InputBinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AbilityLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UChaliceAbility> AbilityClass;
};



/**
 * Base character for ChaliceGame
 */
UCLASS()
class CHALICEGAME_API AChaliceCharacter : public ACharacter, public IAbilitySystemInterface, public IChaliceAbilityInterface
{
	GENERATED_BODY()

public:
	AChaliceCharacter();

	
	// Component name constants

	static FName SpringArmComponentName;
	static FName CameraComponentName;
	static FName AbilityComponentName;
	

	// Character overrides

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;


	// Ability interfaces

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UChaliceAbilityComponent* GetChaliceAbilityComponent() const override { return AbilityComponent; }


	// Control functions

	/**
	 * Applies a movement input relative to the character's control rotation
	 */
	UFUNCTION(BlueprintCallable, Category="BaseCharacter|Control")
	void AddMovementInputControlSpace(FVector ControlSpaceInput, float Scale, bool bForce);
	
	UFUNCTION(BlueprintCallable, Category="BaseCharacter|Control")
	void MoveForward(float Scale);

	UFUNCTION(BlueprintCallable, Category="BaseCharacter|Control")
	void MoveRight(float Scale);


	// Combat functions

	UFUNCTION(BlueprintCallable, Category="BaseCharacter|Combat")
	virtual bool TargetRequirementsMet(const FGameplayTagContainer& TargetTags) const;

	UFUNCTION(BlueprintCallable, Category="BaseCharacter|Combat")
	virtual bool InterruptRequirementsMet(const FGameplayTagContainer& TargetTags) const;


	// Component accessors

	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	TArray<FStartingAbilityInfo> StartingAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	FGameplayTagContainer StartingTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	FGameplayTag GroundedTag;

	// Only trigger weapon hit events for targets with all of the following tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	FGameplayTagQuery TargetRequirements;

	// If any object hit during a weapon trace meets these requirements, ignore all other collisions (useful for shields)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	FGameplayTagQuery InterruptRequirements;

	// Default collision profile to use for weapons. Can be optional overridden by individual weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	FCollisionProfileName WeaponCollisionProfile;


protected:

	// Ability helper functions
	
	/**
	 * Grants all abilities in the starting abilities array. Checks to make sure it only happens once
	 */
	void GrantStartingAbilities();

	/**
	 * Grants all default owned tags. Checks to make sure it only happens once
	 */
	void GrantStartingTags();


private:

	// Default components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCharacter", meta=(AllowPrivateAccess=true))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCharacter", meta=(AllowPrivateAccess=true))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCharacter", meta=(AllowPrivateAccess=true))
	UChaliceAbilityComponent* AbilityComponent;


	// Ability information

	bool bGrantedStartingAbilities = false;
	bool bGrantedStartingTags = false;
	
};
