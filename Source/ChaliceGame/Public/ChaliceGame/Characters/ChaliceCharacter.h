// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"
#include "ChaliceGame/Characters/InputBindings.h"
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


	// Component accessors

	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCharacter|Abilities")
	TArray<FStartingAbilityInfo> StartingAbilities;


protected:

	// Ability helper functions
	
	/**
	 * Grants all abilities in the starting abilities array. Checks to make sure it only happens once
	 */
	void GrantStartingAbilities();


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
	
};
