// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ChaliceAbilityComponent.generated.h"


/**
 * Game specific specialization of the generic ability system component
 */
UCLASS()
class CHALICEABILITIES_API UChaliceAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	// Ability system component overrides

	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo) override;
	virtual bool GetShouldTick() const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Tag utilities

	// Adds a loose tag only if it doesn't exactly match any existing gameplay tags on this component
	UFUNCTION(BlueprintCallable, Category="ChaliceAbilityComponent|Tags")
	void AddLooseGameplayTagUnique(const FGameplayTag& Tag);

	// Removes all loose gameplay tags that exactly match the given tag
	UFUNCTION(BlueprintCallable, Category="ChaliceAbilityComponent|Tags")
	void RemoveLooseGameplayTagFully(const FGameplayTag& Tag);


	// Effect utilities

	UFUNCTION(BlueprintCallable, Category="ChaliceAbilityComponent|Effect")
	FGameplayEffectContextHandle MakeEffectContextFromEvent(FGameplayEventData EventData);
	
	
	// Input controls

	/**
	 * Struct for tracking buffered input
	 */
	struct FBufferedInput
	{
		FVector MovementInput;
		int32 InputID;
		float BufferedTime;
	};

	/**
	 * Returns the movement input from the avatar actor as a character, or any buffered input if called from an
	 * input buffered ability activation
	 */
	UFUNCTION(BlueprintCallable, Category="ChaliceAbilityComponent")
	FVector GetLastMovementInput() const;

	const TArray<FBufferedInput>& GetBufferedInputs() const { return InputBuffer; }
	UEnum* GetInputBindEnum() const { return InputBindEnum; }
	

private:

	// Input buffering

	void BufferInput(int32 InputID);
	void PopInput();
	void PollBufferedInputs();
	
	TArray<FBufferedInput> InputBuffer;
	bool bUsingBufferedMovementInput = false;
	FVector BufferedMovementInput;

	UPROPERTY(BlueprintReadOnly, Category="ChaliceAbilityComponent", meta=(AllowPrivateAccess=true))
	UEnum* InputBindEnum;
	
};
