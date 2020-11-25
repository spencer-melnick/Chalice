// Copyright (c) 2020 Spencer Melnick

#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"


/**
 * Debugger category for game specific abilities, including input buffer
 */
class FGameplayDebuggerCategory_ChaliceAbilities : public FGameplayDebuggerCategory
{
public:
	
	FGameplayDebuggerCategory_ChaliceAbilities();


	// Gameplay debugger overrides

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();


protected:

	struct FRepData
	{
		bool bInputBufferEnabled;
		float InputBufferTime;
		uint32 InputBufferSize;
		
		struct FInputBindingDebug
		{
			int32 InputID;
			FString InputName;

			static FInputBindingDebug CreateBinding(int32 InputID, const UEnum* InputBindEnum);
		};
		
		struct FBufferedInputDebug
		{
			FVector MovementInput;
			FInputBindingDebug InputBinding;
			float TimeLeft;
		};
		TArray<FBufferedInputDebug> BufferedInputs;

		struct FChaliceAbilityDebug
		{
			FString Ability;
			FString Source;
			int32 Level;
			bool bIsActive;
			bool bCanActivate;
			FInputBindingDebug InputBinding;
		};
		TArray<FChaliceAbilityDebug> Abilities;

		void Serialize(FArchive& Ar);
	};

	FRepData DataPack;
};

#endif
