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

	// Editor properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ChaliceAbility")
	bool bBufferInput = false;
};
