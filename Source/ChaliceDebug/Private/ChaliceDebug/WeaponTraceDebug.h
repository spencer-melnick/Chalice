// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"


// Forward declarations

class UWeaponTraceComponent;


/**
 * Static methods used for rendering debug information about weapon trace components
 * Enabled/disabled using "showdebug WeaponTrace"
 */
class FWeaponTraceDebug
{
public:
	
	static void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:

	static void DrawDebugInfo(UWeaponTraceComponent* TraceComponent, AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);
};
