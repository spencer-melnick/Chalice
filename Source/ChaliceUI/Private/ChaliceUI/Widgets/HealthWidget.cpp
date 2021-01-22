// Copyright (c) 2020 Spencer Melnick

#include "ChaliceUI/Widgets/HealthWidget.h"
#include "ChaliceUI/Widgets/HeartWidget.h"
#include "ChaliceUI.h"
#include "GameplayEffectTypes.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"


// UHealthWidget

// User widget overrides

void UHealthWidget::NativeOnInitialized()
{
	// Assign the extra bound attributes here so UAttributeWidget::NativeOnInitialized doesn't miss them
	BoundAttributes.AddUnique(MaxHealthAttribute);
	BoundAttributes.AddUnique(HealthAttribute);
	
	Super::NativeOnInitialized();
}

void UHealthWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsDesignTime())
	{
		RecreateHearts(FMath::CeilToInt(PreviewMaxHealth));
		SetHealth(PreviewHealth);
	}
}


// Attribute widget overrides

void UHealthWidget::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	Super::OnAttributeChanged(ChangeData);

	if (ChangeData.Attribute == MaxHealthAttribute)
	{
		RecreateHearts(FMath::CeilToInt(ChangeData.NewValue));
	}
	else if (ChangeData.Attribute == HealthAttribute)
	{
		SetHealth(ChangeData.NewValue);
	}
}


// Widget helpers

void UHealthWidget::RecreateHearts(int32 MaxHealth)
{
	CHECK_WIDGETS(Panel && HeartWidgetClass)
	
	Panel->ClearChildren();
	for (int32 Index = 0; Index < MaxHealth; Index++)
	{
		UHeartWidget* NewHeart = CreateWidget<UHeartWidget>(this, HeartWidgetClass);
		check(NewHeart)
		UPanelSlot* NewSlot = Panel->AddChild(NewHeart);
		UpdateSlotParameters(NewSlot);
	}
}

void UHealthWidget::SetHealth(float Health)
{
	CHECK_WIDGETS(Panel)

	TArray<UWidget*> ChildHearts = Panel->GetAllChildren();
	for (int32 Index = 0; Index < ChildHearts.Num(); Index++)
	{
		UHeartWidget* ChildHeart = Cast<UHeartWidget>(ChildHearts[Index]);

		if (!ChildHeart)
		{
			UE_LOG(LogChaliceUI, Warning, TEXT("%s child %s [%d] is not a valid heart widget"),
				*GetNameSafe(this), *GetNameSafe(ChildHearts[Index]), Index)
			continue;
		}
		
		const float FillPercent = FMath::Clamp(Health - static_cast<float>(Index), 0.f, 1.f);
		ChildHeart->SetFillPercent(FillPercent);
	}
}

void UHealthWidget::UpdateSlotParameters(UPanelSlot* PanelSlot)
{
	if (!PanelSlot)
	{
		return;
	}
	
	UHorizontalBoxSlot* HorizontalSlot = Cast<UHorizontalBoxSlot>(PanelSlot);
	UVerticalBoxSlot* VerticalSlot = Cast<UVerticalBoxSlot>(PanelSlot);
	if (HorizontalSlot)
	{
		HorizontalSlot->SetHorizontalAlignment(SlotHorizontalAlignment);
		HorizontalSlot->SetVerticalAlignment(SlotVerticalAlignment);
		HorizontalSlot->SetPadding(SlotPadding);
		HorizontalSlot->SetSize(SlotSize);
	}
	else if (VerticalSlot)
	{
		VerticalSlot->SetHorizontalAlignment(SlotHorizontalAlignment);
		VerticalSlot->SetVerticalAlignment(SlotVerticalAlignment);
		VerticalSlot->SetPadding(SlotPadding);
		VerticalSlot->SetSize(SlotSize);
	}
}

