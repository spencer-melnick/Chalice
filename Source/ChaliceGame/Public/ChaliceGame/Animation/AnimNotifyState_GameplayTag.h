// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTags.h"
#include "AnimNotifyState_GameplayTag.generated.h"


// Forward declarations

class UChaliceAbilityComponent;


/**
 * Anim notify state that adds a gameplay tag to the skeletal mesh owner's ability system component (if they have one)
 */
UCLASS()
class CHALICEGAME_API UAnimNotifyState_GameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	// Anim notify overrides
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual FString GetNotifyName_Implementation() const override;


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;


protected:

	// Helpers

	static UChaliceAbilityComponent* GetAbilityComponentFromMesh(USkeletalMeshComponent* MeshComponent);
	
};
