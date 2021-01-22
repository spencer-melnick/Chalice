// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeartWidget.generated.h"


// Forward declarations

class UImage;


/**
 * Simple widget that shows a heart filled at different percentages
 */
UCLASS(Abstract)
class CHALICEUI_API UHeartWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Widget overrides

	virtual void SynchronizeProperties() override;

	
	// Fill control

	UFUNCTION(BlueprintCallable)
	void SetFillPercent(float FillPercent);


	// Editor properties

	// Material to use when the heart is completely full - if null, will default to the partial material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	UMaterialInterface* FullMaterial;

	// Material to use when the heart is neither completely full nor completely empty - must not be null
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	UMaterialInterface* PartialMaterial;

	// Material to use when the heart is completely empty - if null, will default to the partial material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	UMaterialInterface* EmptyMaterial;

	// Name of the scalar parameter in the partial material to be assigned the fill percentage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	FName PercentageParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display")
	bool bHiddenOnEmpty = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heart Display", meta=(ClampMin="0", ClampMax="1"))
	float PreviewFillPercent = 1.f;


protected:

	// Material helpers

	// Sets the image brush material to the material, with error checks
	void SetHeartMaterial(UMaterialInterface* DesiredMaterial, float FillPercent);

	// Sets the image brush material to the partial material with the specified fill percent
	void SetHeartMaterial(float FillPercent);


private:

	// Stored fill

	float LastFillPercent = 1.f;
	

	// Child widgets

	UPROPERTY(meta=(BindWidget))
	UImage* HeartIcon;


	// Stored dynamic materials

	UPROPERTY()
	UMaterialInstanceDynamic* PartialMaterialInstance;
	
};
