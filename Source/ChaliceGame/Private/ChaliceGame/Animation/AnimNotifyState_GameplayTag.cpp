// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Animation/AnimNotifyState_GameplayTag.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"


// UAnimNotifyState_GameplayTag

// Anim notify overrides

void UAnimNotifyState_GameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	UChaliceAbilityComponent* AbilityComponent = GetAbilityComponentFromMesh(MeshComp);
	if (AbilityComponent)
	{
		AbilityComponent->AddLooseGameplayTag(Tag);
	}
}

void UAnimNotifyState_GameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UChaliceAbilityComponent* AbilityComponent = GetAbilityComponentFromMesh(MeshComp);
	if (AbilityComponent)
	{
		AbilityComponent->RemoveLooseGameplayTag(Tag);
	}
}

FString UAnimNotifyState_GameplayTag::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("GameplayTag: {0}"), {Tag.ToString()});
}


// Helpers

UChaliceAbilityComponent* UAnimNotifyState_GameplayTag::GetAbilityComponentFromMesh(USkeletalMeshComponent* MeshComponent)
{
	if (!MeshComponent)
	{
		return nullptr;
	}

	IChaliceAbilityInterface* AbilityInterface = Cast<IChaliceAbilityInterface>(MeshComponent);
	if (!AbilityInterface)
	{
		AbilityInterface = Cast<IChaliceAbilityInterface>(MeshComponent->GetOwner());
	}

	if (AbilityInterface)
	{
		return AbilityInterface->GetChaliceAbilityComponent();
	}

	return nullptr;
}
