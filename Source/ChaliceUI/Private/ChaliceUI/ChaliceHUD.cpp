// Copyright (c) 2020 Spencer Melnick

#include "ChaliceUI/ChaliceHUD.h"
#include "ChaliceUI.h"
#include "Blueprint/UserWidget.h"


// AChaliceHUD

// Actor overrides

void AChaliceHUD::BeginPlay()
{
	Super::BeginPlay();

	SpawnWidgets();
}


// Widget helpers

void AChaliceHUD::SpawnWidgets()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	
	if (!PlayerController)
	{
		UE_LOG(LogChaliceUI, Error, TEXT("%s failed to spawn widgets - no owning player controller"),
			*GetNameSafe(this))
		return;
	}
	
	for (const TSubclassOf<UUserWidget> WidgetClass : StartingWidgets)
	{
		if (!WidgetClass)
		{
			continue;
		}

		UUserWidget* NewWidget = CreateWidget(PlayerController, WidgetClass);
		check(NewWidget)
		NewWidget->AddToViewport();
	}
}

