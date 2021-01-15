// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "AttributeWidget.generated.h"


// Forward declarations

struct FOnAttributeChangeData;


// Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWidgetAttributeChanged, FGameplayAttribute, Attribute, float, OldValue, float, NewValue);


/**
 * Widget that binds to any attributes on the player's ability component and updates when they are changed
 */
UCLASS(Blueprintable, Abstract)
class CHALICEUI_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// User widget overrides
	
	virtual void NativeOnInitialized() override;


	// Editor properties
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayAttribute> BoundAttributes;


	// Delegates

	UPROPERTY(BlueprintAssignable, meta=(DisplayName="On Attribute Changed"))
	FWidgetAttributeChanged OnAttributeChangedDelegate;


protected:

	// Callbacks

	UFUNCTION()
	virtual void OnPlayerPossessPawn(APawn* Pawn);

	UFUNCTION()
	virtual void OnPlayerUnpossessPawn(APawn* Pawn);

	virtual void OnAttributeChanged(const FOnAttributeChangeData& ChangeData);


private:
	
	// Bound delegates
	
	TArray<TPair<FGameplayAttribute, FDelegateHandle>> DelegateHandles;
	
};
