// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbility.h"
#include "AttackAbility.generated.h"


/**
 * Simple ability that handles common attack functionality - namely playing a montage and applying a damage effect to
 * any hit enemies, with blueprint implementable events
 */
UCLASS()
class UAttackAbility : public UChaliceAbility
{
	GENERATED_BODY()

public:

	UAttackAbility();


	// Ability overrides

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;


	// Editor properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	float MontageRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	FName MontageSection;


protected:

	// Callbacks

	UFUNCTION()
	virtual void OnHitEnemy(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnAnimationEnd(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Hit Enemy"))
    void BP_OnHitEnemy(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Animation End"))
    void BP_OnAnimationEnd();

};
