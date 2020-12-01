// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Combat/WeaponBase.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"


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

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (GetWorld() && !GetWorld()->IsGameWorld())
	{
		UpdateEditorSpheres();
	}
#endif
}


// Weapon controls

void AWeaponBase::EnableTrace()
{
	bTraceEnabled = true;
	UpdateTraceLocations();
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

void AWeaponBase::UpdateTraceLocations()
{
	for (FWeaponTraceShape& Shape : TraceShapes)
	{
		Shape.LastPosition = GetTraceShapeLocation(Shape);
	}
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


// Editor display

#if WITH_EDITOR

void AWeaponBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (GetWorld() && !GetWorld()->IsGameWorld() && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AWeaponBase, TraceShapes))
	{
		UpdateEditorSpheres();
	}
}

void AWeaponBase::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (GetWorld() && !GetWorld()->IsGameWorld() && PropertyChangedEvent.PropertyChain.GetHead()->GetValue()->NamePrivate == GET_MEMBER_NAME_CHECKED(AWeaponBase, TraceShapes))
	{
		UpdateEditorSpheres();
	}
}

void AWeaponBase::UpdateEditorSpheres()
{
	UpdateTraceLocations();
	EditorSpheres.SetNumZeroed(TraceShapes.Num());
	
	for (int32 Index = 0; Index < TraceShapes.Num(); Index++)
	{
		FWeaponTraceShape& TraceShape = TraceShapes[Index];
		USphereComponent*& SphereComponent = EditorSpheres[Index];
		
		if (!SphereComponent)
		{
			SphereComponent = NewObject<USphereComponent>(this);
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SphereComponent->bSelectable = false;
			SphereComponent->bIsEditorOnly = true;
			SphereComponent->ShapeColor = FColor::Cyan;
			SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}

		SphereComponent->SetRelativeLocation(TraceShape.LastPosition);
		SphereComponent->SetSphereRadius(TraceShape.Radius);
	}
}

#endif
