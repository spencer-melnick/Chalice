// Copyright (c) 2020 Spencer Melnick

#include "ChaliceUI/Widgets/AttributeWidget.h"
#include "ChaliceUI.h"
#include "ChaliceGame/Player/ChalicePlayerController.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"


// UAttributeWidget

// Widget overrides

void UAttributeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AChalicePlayerController* PlayerController = GetOwningPlayer<AChalicePlayerController>();
	if (!PlayerController)
	{
		UE_LOG(LogChaliceUI, Warning, TEXT("%s cannot bind attribute delegates - %s is not a valid Chalice player controller"),
			*GetNameSafe(this), *GetNameSafe(GetOwningPlayer()))
		return;
	}

	PlayerController->OnPossessDelegate.AddUniqueDynamic(this, &UAttributeWidget::OnPlayerPossessPawn);
	PlayerController->OnUnpossessDelegate.AddUniqueDynamic(this, &UAttributeWidget::OnPlayerUnpossessPawn);

	APawn* PossessedPawn = GetOwningPlayerPawn();
	if (PossessedPawn)
	{
		// Call the delegate even if the pawn is already possessed when this widget is created
		OnPlayerPossessPawn(PossessedPawn);
	}
}


// Callbacks

void UAttributeWidget::OnPlayerPossessPawn(APawn* Pawn)
{
	UChaliceAbilityComponent* AbilityComponent = UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Pawn);
	if (!AbilityComponent)
	{
		return;
	}

	for (const FGameplayAttribute Attribute : BoundAttributes)
	{
		FDelegateHandle DelegateHandle = AbilityComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UAttributeWidget::OnAttributeChanged);
		DelegateHandles.Emplace(Attribute, DelegateHandle);
	}
}

void UAttributeWidget::OnPlayerUnpossessPawn(APawn* Pawn)
{
	UChaliceAbilityComponent* AbilityComponent = UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Pawn);
	if (!AbilityComponent)
	{
		return;
	}

	for (auto& DelegateHandle : DelegateHandles)
	{
		AbilityComponent->GetGameplayAttributeValueChangeDelegate(DelegateHandle.Key).Remove(DelegateHandle.Value);
	}
}

void UAttributeWidget::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_BP(ChangeData.Attribute, ChangeData.OldValue, ChangeData.NewValue);
	OnAttributeChangedDelegate.Broadcast(ChangeData.Attribute, ChangeData.OldValue, ChangeData.NewValue);
}
