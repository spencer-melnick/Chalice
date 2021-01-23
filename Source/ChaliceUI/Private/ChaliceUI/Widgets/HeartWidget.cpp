// Copyright (c) 2020 Spencer Melnick

#include "ChaliceUI/Widgets/HeartWidget.h"
#include "ChaliceUI.h"
#include "Components/Image.h"


// UHeartWidget

// Widget overrides

void UHeartWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (PartialMaterial)
	{
		PartialMaterialInstance = UMaterialInstanceDynamic::Create(PartialMaterial, this);
	}

	if (IsDesignTime())
	{
		SetFillPercent(PreviewFillPercent);
	}
	else
	{
		SetFillPercent(LastFillPercent);
	}
}


// Fill control

void UHeartWidget::SetFillPercent(float FillPercent)
{
	CHECK_WIDGETS(HeartIcon)
	
	FillPercent = FMath::Clamp(FillPercent, 0.f, 1.f);
	LastFillPercent = FillPercent;
	
	if (FMath::IsNearlyZero(FillPercent))
	{
		SetHeartMaterial(EmptyMaterial, 0.f);
		if (bHiddenOnEmpty)
		{
			HeartIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		HeartIcon->SetVisibility(ESlateVisibility::Visible);

		if (FMath::IsNearlyEqual(FillPercent, 1.f))
		{
			SetHeartMaterial(FullMaterial, 1.f);
		}
		else
		{
			SetHeartMaterial(FillPercent);
		}
	}
}


// Material helpers

void UHeartWidget::SetHeartMaterial(UMaterialInterface* DesiredMaterial, float FillPercent)
{
	CHECK_WIDGETS(HeartIcon)

	if (DesiredMaterial)
	{
		HeartIcon->Brush.SetResourceObject(DesiredMaterial);
	}
	else
	{
		SetHeartMaterial(FillPercent);
	}
}

void UHeartWidget::SetHeartMaterial(float FillPercent)
{
	CHECK_WIDGETS(HeartIcon)
	
	if (!PartialMaterialInstance)
	{
		UE_LOG(LogChaliceUI, Warning, TEXT("%s failed on %s - no partial material instance dynamic"),
            ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	HeartIcon->Brush.SetResourceObject(PartialMaterialInstance);
	PartialMaterialInstance->SetScalarParameterValue(PercentageParameterName, FillPercent);
}

