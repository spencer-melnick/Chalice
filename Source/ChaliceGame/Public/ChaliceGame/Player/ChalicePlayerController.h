// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "ChalicePlayerController.generated.h"


// Forward declarations

class AChaliceCharacter;


// Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPossessionDelegate, APawn*, Character);


/**
 * Main game specific player controller class
 */
UCLASS()
class CHALICEGAME_API AChalicePlayerController : public APlayerController, public IAbilitySystemInterface, public IChaliceAbilityInterface
{
	GENERATED_BODY()

public:

	AChalicePlayerController();


	// Player controller overrides

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


	// Ability interface overrides

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UChaliceAbilityComponent* GetChaliceAbilityComponent() const override;


	// Accessors

	AChaliceCharacter* GetChaliceCharacter() const;


	// Delegates

	UPROPERTY(BlueprintAssignable, meta=(DisplayName="On Possess"))
	FPlayerPossessionDelegate OnPossessDelegate;

	UPROPERTY(BlueprintAssignable, meta=(DisplayName="On Possess"))
	FPlayerPossessionDelegate OnUnpossessDelegate;
	
};
