// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbilities/Abilities/ChaliceAbility.h"
#include "DodgeAbility.generated.h"


/**
 * Simple base for the dodge ability, without any additional tags or requirements
 */
UCLASS()
class UDodgeAbility : public UChaliceAbility
{
	GENERATED_BODY()

public:

	UDodgeAbility();


	// Ability overrides

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;


	// Editor properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	float MontageRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	FName StartingSection = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	UCurveFloat* PositionCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	float DodgeDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DodgeAbility")
	float DodgeDistance = 50.f;


protected:

	// Callback functions

	UFUNCTION()
	void DodgeFinished();
};
