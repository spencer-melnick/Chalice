// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "ChalicePlayerController.generated.h"


// Forward declarations

class AChaliceCharacter;


/**
 * Main game specific player controller class
 */
UCLASS()
class CHALICEGAME_API AChalicePlayerController : public APlayerController, public IAbilitySystemInterface, public IChaliceAbilityInterface
{
	GENERATED_BODY()

public:

	AChalicePlayerController();


	// Ability interface overrides

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UChaliceAbilityComponent* GetChaliceAbilityComponent() const override;


	// Accessors

	AChaliceCharacter* GetChaliceCharacter() const;
	
};
