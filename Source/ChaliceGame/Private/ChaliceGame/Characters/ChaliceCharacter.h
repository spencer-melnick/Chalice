// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChaliceCharacter.generated.h"


// Forward declarations

class USpringArmComponent;
class UCameraComponent;


/**
 * Base character for ChaliceGame
 */
UCLASS()
class AChaliceCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AChaliceCharacter();

	
	// Component name constants

	static FName SpringArmComponentName;
	static FName CameraComponentName;
	

	// Character overrides

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;


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


private:

	// Default components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCharacter", meta=(AllowPrivateAccess=true))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCharacter", meta=(AllowPrivateAccess=true))
	UCameraComponent* CameraComponent;
	
};
