// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Combat/WeaponTraceComponent.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceGame.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"


// UWeaponTraceComponent

UWeaponTraceComponent::UWeaponTraceComponent()
{
	// Enable ticking on activation
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Component overrides

void UWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bTraceEnabled)
	{
		TraceWeapon();
	}
}


// Trace controls

void UWeaponTraceComponent::EnableTrace()
{
	bTraceEnabled = true;
	UpdateTraceLocations();
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UWeaponTraceComponent::DisableTrace()
{
	bTraceEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}


// Accessors

AChaliceCharacter* UWeaponTraceComponent::GetWeaponOwner() const
{
	return Cast<AChaliceCharacter>(GetOwner()->GetAttachParentActor());
}


// Helper functions

FTransform UWeaponTraceComponent::GetTraceShapeTransform(const FWeaponTraceShape& TraceShape) const
{
	const FTransform ComponentTransform = GetComponentTransform();
	if (TraceShape.Socket == NAME_None)
	{
		return ComponentTransform;
	}
	
	const USceneComponent* ParentComponent = GetAttachParent();
	if (!ParentComponent)
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed - %s has no attached parent"),
            ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return ComponentTransform;
	}
	
	if (!ParentComponent->DoesSocketExist(TraceShape.Socket))
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed on %s - %s has no socket named %s"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(ParentComponent), *TraceShape.Socket.ToString())
		return ComponentTransform;
	}
	
	return ParentComponent->GetSocketTransform(TraceShape.Socket);
}

FVector UWeaponTraceComponent::GetTraceShapeLocation(const FWeaponTraceShape& TraceShape) const
{
	return GetTraceShapeTransform(TraceShape).TransformPosition(TraceShape.Location);
}

void UWeaponTraceComponent::UpdateTraceLocations()
{
	for (FWeaponTraceShape& Shape : TraceShapes)
	{
		Shape.LastPosition = GetTraceShapeLocation(Shape);
	}
}

FGameplayEventData UWeaponTraceComponent::CreateEventFromTrace(const FHitResult& HitResult, const FWeaponTraceShape& Shape, const FGameplayTag EventTag) const
{
	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.Instigator = GetOwner();
	EventData.Target = HitResult.Actor.Get();
	EventData.InstigatorTags = UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(HitResult.GetActor());
	EventData.InstigatorTags.AppendTags(Shape.Tags);
	EventData.InstigatorTags.AppendTags(WeaponTags);
	EventData.TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	return EventData;
}

void UWeaponTraceComponent::TraceWeapon()
{
	TArray<FGameplayEventData> HitEvents;
	bool bInterrupted = false;
	
	for (FWeaponTraceShape& TraceShape : TraceShapes)
	{
		const FVector OldShapeLocation = TraceShape.LastPosition;
		const FVector NewShapeLocation = GetTraceShapeLocation(TraceShape);

		if (!bInterrupted)
		{
			TArray<FHitResult> HitResults;
			GetWorld()->SweepMultiByProfile(HitResults, OldShapeLocation, NewShapeLocation, FQuat::Identity, TraceProfile, FCollisionShape::MakeSphere(TraceShape.Radius));

			for (const FHitResult& HitResult : HitResults)
			{
				if (MeetsInterruptRequirements(HitResult))
				{
					// Dump all hit events except the one that interrupted our trace
					bInterrupted = true;
					HitEvents.Empty(1);
					HitEvents.Add(CreateEventFromTrace(HitResult, TraceShape, InterruptEventTag));
				}
				else if (MeetsTargetRequirements(HitResult))
				{
					HitEvents.Add(CreateEventFromTrace(HitResult, TraceShape, HitEventTag));
				}
			}
		}
	}

	if (bInterrupted)
	{
		check(HitEvents.Num() == 1);
		OnTraceInterrupt(HitEvents[0]);
	}
	else
	{
		OnTraceHit(HitEvents);
	}
}

bool UWeaponTraceComponent::MeetsTargetRequirements(const FHitResult& HitResult)
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	return TargetRequirements.RequirementsMet(UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor));
}

bool UWeaponTraceComponent::MeetsInterruptRequirements(const FHitResult& HitResult)
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	return InterruptRequirements.RequirementsMet(UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor));
}


// Events

void UWeaponTraceComponent::OnTraceHit(TArray<FGameplayEventData>& Events)
{
	AChaliceCharacter* WeaponOwner = GetWeaponOwner();
	for (FGameplayEventData& Event : Events)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(WeaponOwner, HitEventTag, Event);
		// Why can't the actor be const? >:(
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), HitEventTag, Event);
	}
}

void UWeaponTraceComponent::OnTraceInterrupt(FGameplayEventData Event)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetWeaponOwner(), InterruptEventTag, Event);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), InterruptEventTag, Event);
}
