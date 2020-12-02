// Copyright (c) 2020 Spencer Melnick

#include "ChaliceDebug/WeaponTraceDebug.h"
#include "ChaliceGame/Combat/WeaponTraceComponent.h"
#include "GameFramework/HUD.h"
#include "DisplayDebugHelpers.h"
#include "DrawDebugHelpers.h"


// FWeaponTraceDebug

void FWeaponTraceDebug::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
	if (!DisplayInfo.IsDisplayOn(TEXT("WeaponTrace")))
	{
		return;
	}

	for (TObjectIterator<UWeaponTraceComponent> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() == HUD->GetWorld())
		{
			DrawDebugInfo(*Itr, HUD, Canvas, DisplayInfo, YL, YPos);
		}
	}
}

void FWeaponTraceDebug::DrawDebugInfo(UWeaponTraceComponent* TraceComponent, AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
	check(TraceComponent);
	
	const bool bTraceEnabled = TraceComponent->IsTraceEnabled();
	const FColor ShapeColor = bTraceEnabled ? FColor(184, 224, 255) : FColor::Cyan;
	
    if (bTraceEnabled)
    {
        for (const FVector& HitLocation : TraceComponent->LastHitLocations)
        {
            DrawDebugPoint(HUD->GetWorld(), HitLocation, 10.f, FColor::Green, false, 0.3f);
        }
    }

	for (const FWeaponTraceShape& TraceShape : TraceComponent->TraceShapes)
	{
		const FVector TraceLocation = TraceComponent->GetTraceShapeLocation(TraceShape);
		DrawDebugSphere(HUD->GetWorld(), TraceLocation, TraceShape.Radius, 16, ShapeColor);

		if (bTraceEnabled)
		{
			DrawDebugLine(HUD->GetWorld(), TraceShape.PreviousPosition, TraceLocation, FColor::Red, false, 0.3f, 0, 1.f);
		}
	}
}

