// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChaliceHUD.generated.h"


// Forward declarations

class UUserWidget;


/**
 * Base class for custom HUD - spawns widgets upon creation
 */
UCLASS(Abstract)
class CHALICEUI_API AChaliceHUD : public AHUD
{
	GENERATED_BODY()

public:

	// Actor overrides

	virtual void BeginPlay() override;


	// Accessors

	const TArray<UUserWidget*>& GetWidgets() const { return Widgets; }


	// Editor properties

	UPROPERTY(EditAnywhere, Category="Widgets")
	TArray<TSubclassOf<UUserWidget>> StartingWidgets;


private:

	// Widget helpers

	void SpawnWidgets();


	// Widgets

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<UUserWidget*> Widgets;
	
};
