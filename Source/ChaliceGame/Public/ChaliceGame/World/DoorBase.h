// Copyright (c) 2020 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"

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
	
private:
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Alpha;

	// Percent changed per second, 100 will close the door in 1 second, etc.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EDoorState State;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FTransform OpenTransform;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FTransform ClosedTransform;

	// The origin point for the relative transformations
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))	
	USceneComponent* Origin;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* TransformCurve;

	float GetDelta(float DeltaSeconds);
};
