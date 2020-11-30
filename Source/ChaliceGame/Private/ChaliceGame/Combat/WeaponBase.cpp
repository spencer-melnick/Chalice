// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Combat/WeaponBase.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"


// AWeaponBase

AWeaponBase::AWeaponBase()
{
	// Enable ticking on activation
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create default components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(MeshComponentName);
	RootComponent = MeshComponent;
}


// Component name constants

FName AWeaponBase::MeshComponentName(TEXT("MeshComponent"));


// Actor overrides

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaSeconds)
{
	if (bTraceEnabled)
	{
		TraceWeapon();
	}
}


// Weapon controls

void AWeaponBase::EnableTrace()
{
	bTraceEnabled = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AWeaponBase::DisableTrace()
{
	bTraceEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}


// Accessors

AChaliceCharacter* AWeaponBase::GetWeaponOwner() const
{
	return Cast<AChaliceCharacter>(GetAttachParentActor());
}


// Helper functions

FVector AWeaponBase::GetTraceShapeLocation(const FWeaponTraceShape& TraceShape) const
{
	const FTransform SocketTransform = MeshComponent->GetSocketTransform(TraceShape.Socket);
	return SocketTransform.TransformVector(TraceShape.Location);
}

FGameplayEventData AWeaponBase::CreateEventFromTrace(const FHitResult& HitResult, const FWeaponTraceShape& Shape, const FGameplayTag EventTag) const
{
	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.Instigator = this;
	EventData.Target = HitResult.Actor.Get();
	EventData.InstigatorTags = UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(HitResult.GetActor());
	EventData.InstigatorTags.AppendTags(Shape.Tags);
	EventData.InstigatorTags.AppendTags(WeaponTags);
	EventData.TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	return EventData;
}

void AWeaponBase::TraceWeapon()
{
	TArray<FGameplayEventData> HitEvents;
	bool bInterrupted = false;
	
	for (FWeaponTraceShape& TraceShape : TraceShapes)
	{
		const FVector OldShapeLocation = TraceShape.LastPosition;
		const FVector NewShapeLocation = GetTraceShapeLocation(TraceShape);

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
				break;
			}
			
			if (MeetsTargetRequirements(HitResult))
			{
				HitEvents.Add(CreateEventFromTrace(HitResult, TraceShape, HitEventTag));
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

bool AWeaponBase::MeetsTargetRequirements(const FHitResult& HitResult)
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	return TargetRequirements.RequirementsMet(UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor));
}

bool AWeaponBase::MeetsInterruptRequirements(const FHitResult& HitResult)
{
	AActor* Actor = HitResult.GetActor();
	if (!Actor || !UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(Actor))
	{
		return false;
	}
	return InterruptRequirements.RequirementsMet(UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(Actor));
}


// Events

void AWeaponBase::OnTraceHit(TArray<FGameplayEventData>& Events)
{
	AChaliceCharacter* WeaponOwner = GetWeaponOwner();
	for (FGameplayEventData& Event : Events)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(WeaponOwner, HitEventTag, Event);
		// Why can't the actor be const? >:(
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), HitEventTag, Event);
	}

	BP_OnTraceHit(Events);
}

void AWeaponBase::OnTraceInterrupt(FGameplayEventData Event)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetWeaponOwner(), InterruptEventTag, Event);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Event.Target), InterruptEventTag, Event);

	BP_OnTraceInterrupt(Event);
}

