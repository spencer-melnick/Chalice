// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAttack,
	SecondaryAttack,
	Dodge,
	Jump,
	Interact
};
