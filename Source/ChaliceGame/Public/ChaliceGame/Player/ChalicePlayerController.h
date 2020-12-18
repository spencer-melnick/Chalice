// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChalicePlayerController.generated.h"


/**
 * Main game specific player controller class
 */
UCLASS()
class CHALICEGAME_API AChalicePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AChalicePlayerController();


	// Component name constants

	static FName InteractionComponentName;
	
};
