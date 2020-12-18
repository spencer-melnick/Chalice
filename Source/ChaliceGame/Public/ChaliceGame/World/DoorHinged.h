// Copyright (c) 2020 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaliceGame/World/DoorBase.h"
#include "DoorHinged.generated.h"

UCLASS()
class CHALICEGAME_API ADoorHinged : public ADoorBase
{
	GENERATED_BODY()

public:
	ADoorHinged();
	virtual void Open() override;
	virtual void Close() override;
};
