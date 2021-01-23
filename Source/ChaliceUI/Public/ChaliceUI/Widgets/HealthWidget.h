// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "AttributeWidget.h"
#include "HealthWidget.generated.h"


// Forward declaration

class UHeartWidget;
class UPanelWidget;


/**
 * Widget that displays player health using heart widgets
 */
UCLASS(Abstract)
class CHALICEUI_API UHealthWidget : public UAttributeWidget
{
	GENERATED_BODY()

public:

	// User widget overrides

	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	

	// Attribute widget overrides

	virtual void OnAttributeChanged(const FOnAttributeChangeData& ChangeData) override;


	// Editor properties
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Display")
	FGameplayAttribute MaxHealthAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Display")
	FGameplayAttribute HealthAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Display")
	float PreviewMaxHealth = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Display")
	float PreviewHealth = 5.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	TSubclassOf<UHeartWidget> HeartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	TEnumAsByte<EHorizontalAlignment> SlotHorizontalAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	TEnumAsByte<EVerticalAlignment> SlotVerticalAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	FMargin SlotPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	FSlateChildSize SlotSize;


protected:

	// Widget helpers

	void RecreateHearts(int32 MaxHealth);
	void SetHealth(float Health);
	void UpdateSlotParameters(UPanelSlot* PanelSlot);

	
private:

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* Panel;

	UPROPERTY()
	TArray<UHeartWidget*> HeartWidgets;
	
};
