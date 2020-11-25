// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ChaliceAbility.generated.h"


// Forward declarations

class ACharacter;


/**
 * Game specific specialization for gameplay abilities
 */
UCLASS()
class CHALICEABILITIES_API UChaliceAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:


	// Input controls

	/**
	 * Gets the last movement input from the avatar actor as a character, or the input at the time of input buffering
	 * if the ability was triggered from buffered input
	 */
	UFUNCTION(BlueprintCallable, Category="ChaliceAbility|Input")
	FVector GetLastMovementInput(const FGameplayAbilityActorInfo& ActorInfo) const;
	

	// Editor properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ChaliceAbility")
	bool bBufferInput = false;

};
