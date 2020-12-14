// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Combat/WeaponTraceComponent.h"
#include "ChaliceGame/Combat/WeaponBase.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "ChaliceAbilities/System/ChaliceAbilitySetttings.h"
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
	ClearHitActors();
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UWeaponTraceComponent::DisableTrace()
{
	bTraceEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UWeaponTraceComponent::ClearHitActors()
{
	HitActors.Empty();
}


// Accessors

AWeaponBase* UWeaponTraceComponent::GetWeaponActor() const
{
	return Cast<AWeaponBase>(GetOwner());
}

AChaliceCharacter* UWeaponTraceComponent::GetWeaponOwner() const
{
	const AWeaponBase* Weapon = GetWeaponActor();
	if (Weapon)
	{
		return Weapon->GetCharacterOwner();
	}
	return Cast<AChaliceCharacter>(GetOwner()->GetAttachParentActor());
}

FGameplayTagContainer UWeaponTraceComponent::GetTags() const
{
	FGameplayTagContainer Result;
	const AWeaponBase* WeaponActor = GetWeaponActor();
	if (WeaponActor)
	{
		Result = WeaponActor->GetTags();
	}
	Result.AppendTags(TraceTags);
	return Result;
}

FName UWeaponTraceComponent::GetCollisionProfile() const
{
	const AWeaponBase* WeaponActor = GetWeaponActor();
	if (!WeaponActor || bOverrideWeaponCollisionProfile)
	{
		return CollisionProfile.Name;
	}
	return WeaponActor->GetCollisionProfile();
}


// Helper functions

FTransform UWeaponTraceComponent::GetTraceShapeTransform(const FWeaponTraceShape& TraceShape) const
{
	return GetComponentTransform();
}

FVector UWeaponTraceComponent::GetTraceShapeLocation(const FWeaponTraceShape& TraceShape) const
{
	return GetTraceShapeTransform(TraceShape).TransformPosition(TraceShape.Location);
}

FGameplayTagContainer UWeaponTraceComponent::GetTraceShapeTags(const FWeaponTraceShape& TraceShape) const
{
	FGameplayTagContainer Tags = GetTags();
	Tags.AppendTags(TraceShape.ShapeTags);
	return Tags;
}

void UWeaponTraceComponent::UpdateTraceLocations()
{
	for (FWeaponTraceShape& Shape : TraceShapes)
	{
		Shape.CurrentTraceLocation = GetTraceShapeLocation(Shape);
	}
}

FGameplayEventData UWeaponTraceComponent::CreateEventFromTrace(const FHitResult& HitResult, const FWeaponTraceShape& Shape, const FGameplayTag EventTag) const
{
	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.Instigator = GetOwner();
	EventData.InstigatorTags = GetTraceShapeTags(Shape);
	EventData.Target = HitResult.Actor.Get();

	// Get target tags if the target has an ability component
	const IChaliceAbilityInterface* TargetAbilityInterface = Cast<IChaliceAbilityInterface>(EventData.Target);
	if (TargetAbilityInterface)
	{
		const UChaliceAbilityComponent* TargetAbilityComponent = TargetAbilityInterface->GetChaliceAbilityComponent();
		if (TargetAbilityComponent)
		{
			TargetAbilityComponent->GetOwnedGameplayTags(EventData.TargetTags);
		}
	}
	
	EventData.TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	return EventData;
}

void UWeaponTraceComponent::TraceWeapon()
{
	TArray<FHitEvent> HitEvents;
	bool bInterrupted = false;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetWeaponActor());
	QueryParams.AddIgnoredActor(GetWeaponOwner());
	
	for (FWeaponTraceShape& TraceShape : TraceShapes)
	{
		const FVector OldShapeLocation = TraceShape.CurrentTraceLocation;
		const FVector NewShapeLocation = GetTraceShapeLocation(TraceShape);
		const FName UsedCollisionProfile = GetCollisionProfile();

		TraceShape.CurrentTraceLocation = NewShapeLocation;
		TraceShape.PreviousTraceLocation = OldShapeLocation;

		if (!bInterrupted)
		{
			TArray<FHitResult> HitResults;
			GetWorld()->SweepMultiByProfile(HitResults, OldShapeLocation, NewShapeLocation, FQuat::Identity, UsedCollisionProfile, FCollisionShape::MakeSphere(TraceShape.Radius), QueryParams);

			for (const FHitResult& HitResult : HitResults)
			{
				if (InterruptRequirementsMet(HitResult))
				{
					// Dump all hit events except the one that interrupted our trace
					bInterrupted = true;
					HitEvents.Empty(1);
					HitEvents.Emplace(HitResult, TraceShape);
				}
				else if (TargetRequirementsMet(HitResult))
				{
					HitEvents.Emplace(HitResult, TraceShape);
				}
			}
		}
	}

	if (bInterrupted)
	{
		check(HitEvents.Num() == 1);
		OnTraceInterrupt(CreateEventFromTrace(HitEvents[0].HitResult, HitEvents[0].TraceShape, InterruptEventTag));
	}
	else
	{
		FilterHitEvents(HitEvents);
		for (const auto& HitEvent : HitEvents)
		{
			OnTraceHit(CreateEventFromTrace(HitEvent.HitResult, HitEvent.TraceShape, UChaliceAbilitySettings::Get()->HitEventTag));
		}
	}

	// Track hit locations for debug
	LastHitLocations.Empty(HitEvents.Num());
	for (const FHitEvent& HitEvent : HitEvents)
	{
		LastHitLocations.Add(HitEvent.HitResult.Location);
	}
}

bool UWeaponTraceComponent::TargetRequirementsMet(const FHitResult& HitResult) const
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	const FGameplayTagContainer ActorTags = UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor);
	
	const AWeaponBase* WeaponActor = GetWeaponActor();
	if (WeaponActor && !WeaponActor->TargetRequirementsMet(ActorTags))
	{
		return false;
	}
	return TargetRequirements.IsEmpty() ? true : TargetRequirements.Matches(ActorTags);
}

bool UWeaponTraceComponent::InterruptRequirementsMet(const FHitResult& HitResult) const
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	const FGameplayTagContainer ActorTags = UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor);
	
	const AWeaponBase* WeaponActor = GetWeaponActor();
	if (WeaponActor && !WeaponActor->InterruptRequirementsMet(ActorTags))
	{
		return false;
	}
	return InterruptRequirements.IsEmpty() ? true : InterruptRequirements.Matches(ActorTags);
}

void UWeaponTraceComponent::FilterHitEvents(TArray<FHitEvent>& HitResults)
{
	TMap<AActor*, FHitEvent> PriorityMap;

	for (const FHitEvent& CurrentHitEvent : HitResults)
	{
		AActor* HitActor = CurrentHitEvent.HitResult.GetActor();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			// Prevent duplicate hits until we clear the hit actor list
			continue;
		}
		
		const FHitEvent* FoundHitEvent = PriorityMap.Find(HitActor);
		if (!FoundHitEvent)
		{
			PriorityMap.Add(HitActor, CurrentHitEvent);
		}
		else if (FoundHitEvent->TraceShape.Priority < CurrentHitEvent.TraceShape.Priority)
		{
			PriorityMap[HitActor] = CurrentHitEvent;
		}
	}

	HitResults.Empty(PriorityMap.Num());
	for (const auto& PriorityEvent : PriorityMap)
	{
		HitActors.Add(PriorityEvent.Key);
		HitResults.Add(PriorityEvent.Value);
	}
}


// Events

void UWeaponTraceComponent::OnTraceHit(FGameplayEventData Event)
{
	const FGameplayTag HitEventTag = UChaliceAbilitySettings::Get()->HitEventTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetWeaponOwner(), HitEventTag, Event);
	// Why can't the actor be const? >:(
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), HitEventTag, Event);
}

void UWeaponTraceComponent::OnTraceInterrupt(FGameplayEventData Event)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetWeaponOwner(), InterruptEventTag, Event);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), InterruptEventTag, Event);
}
