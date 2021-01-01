// Copyright (c) 2020 Stephen Melnick

#include "ChaliceGame/World/DoorBase.h"


// ADoorBase

ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Alpha = 0.0f;
	Speed = 100.0f;
	State = EDoorState::Closed;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	Origin->SetupAttachment(RootComponent);
}

void ADoorBase::Open()
{
	if (State == EDoorState::Closed || State == EDoorState::Closing)
	{
		State = EDoorState::Opening;
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

void ADoorBase::Close()
{
	if (State == EDoorState::Open || State == EDoorState::Opening)
	{
		State = EDoorState::Closing;
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

void ADoorBase::Toggle()
{
	switch(State)
	{
		case EDoorState::Closed:
			Open();
			break;
		case EDoorState::Open:
			Close();
			break;
		default:
		break;
	}
}


void ADoorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (State)
	{
		case EDoorState::Closing:
			if (Alpha - GetDelta(DeltaSeconds) <= 0.0f)
			{
				Alpha = 0.0f;
				State = EDoorState::Closed;
				PrimaryActorTick.SetTickFunctionEnable(false);
				Origin->SetRelativeTransform(ClosedTransform);
			}
			else
			{
				Alpha -= GetDelta(DeltaSeconds);
			}
			break;
		
		case EDoorState::Opening:
			if (Alpha + GetDelta(DeltaSeconds) >= 1.0f)
			{
				Alpha = 1.0f;
				State = EDoorState::Open;
				Origin->SetRelativeTransform(OpenTransform);
				PrimaryActorTick.SetTickFunctionEnable(false);
			}
			else
			{
				Alpha += GetDelta(DeltaSeconds);
			}
			break;

		default:
			break;
	}
	FTransform Result = FTransform();
	Result.Blend(ClosedTransform, OpenTransform, TransformCurve->GetFloatValue(Alpha));
	Origin->SetRelativeTransform(Result);
}

float ADoorBase::GetDelta(float DeltaSeconds)
{
	return Speed / 100 * DeltaSeconds;
}
