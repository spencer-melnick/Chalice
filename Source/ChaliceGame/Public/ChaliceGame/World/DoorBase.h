// Copyright (c) 2020 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"


#include "DoorBase.generated.h"

/**
 *
 */
UENUM()
enum class EDoorState : uint8
{
	Open,
	Opening,
    Closed,
	Closing
};

UCLASS(Abstract)
class CHALICEGAME_API ADoorBase : public AActor
{
	GENERATED_BODY()

public:
	ADoorBase();
	UFUNCTION(BlueprintCallable)
	virtual void Open();

	UFUNCTION(BlueprintCallable)
	virtual void Close();
	
	UFUNCTION(BlueprintCallable)
	virtual void Toggle();
	virtual void Tick(float DeltaSeconds) override;


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha;

	// Percent changed per second, 100 will close the door in 1 second, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ELerpInterpolationMode::Type> InterpolationMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform OpenTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ClosedTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TransformCurve;

	
private:

	float GetDelta(float DeltaSeconds) const;

	
	// Private state

	EDoorState State;

	
	// Components

	// The origin point for the relative transformations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	
	USceneComponent* Origin;

};
