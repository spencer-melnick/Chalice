// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ChaliceAbilitiesBlueprintLibrary.generated.h"


// Forward declarations

class UChaliceAbilityComponent;
class UInteractionComponent;


/**
 *
 */
UCLASS()
class CHALICEABILITIES_API UChaliceAbilitiesBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="ChaliceAbilities")
	static UChaliceAbilityComponent* GetAbilityComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintPure, Category="ChaliceAbilities")
	static FGameplayTagContainer GetActorOwnedTags(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category="ChaliceAbilities")
	static UInteractionComponent* GetInteractionComponent(const AActor* Actor);
};
