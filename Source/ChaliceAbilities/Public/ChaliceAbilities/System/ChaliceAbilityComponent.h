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
	virtual bool GetShouldTick() const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	// Input buffering

	void BufferInput(int32 InputID);
	void PopInput();
	void PollBufferedInputs();
	
	struct FBufferedInput
	{
		FVector MovementInput;
		int32 InputID;
		float BufferedTime;
	};
	TArray<FBufferedInput> InputBuffer;
	FVector BufferedMovementInput;
	
};
