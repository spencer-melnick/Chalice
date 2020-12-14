// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/System/GameplayDebuggerCategory_ChaliceAbilities.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitySetttings.h"
#include "AbilitySystemGlobals.h"

// FGameplayDebuggerCategory_ChaliceAbilities

FGameplayDebuggerCategory_ChaliceAbilities::FGameplayDebuggerCategory_ChaliceAbilities()
{
	bShowOnlyWithDebugActor = false;
	SetDataPackReplication<FRepData>(&DataPack);
}


// Gameplay debugger overrides

void FGameplayDebuggerCategory_ChaliceAbilities::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	const UChaliceAbilitySettings* AbilitySettings = UChaliceAbilitySettings::Get();

	DataPack.bInputBufferEnabled = AbilitySettings->bInputBufferEnabled;
	DataPack.InputBufferTime = AbilitySettings->InputBufferTime;
	DataPack.InputBufferSize = AbilitySettings->InputBufferSize;


	if (!DebugActor)
	{
		DebugActor = OwnerPC->GetPawn();
	}
	
	const UChaliceAbilityComponent* AbilityComponent = Cast<UChaliceAbilityComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DebugActor));
	if (AbilityComponent)
	{
		const UEnum* BindingEnum = AbilityComponent->GetInputBindEnum();

		const TArray<UChaliceAbilityComponent::FBufferedInput> BufferedInputs = AbilityComponent->GetBufferedInputs();
		for (const UChaliceAbilityComponent::FBufferedInput& BufferedInput : BufferedInputs)
		{
			FRepData::FBufferedInputDebug ItemData;
			ItemData.MovementInput = BufferedInput.MovementInput;
			ItemData.InputBinding = FRepData::FInputBindingDebug::CreateBinding(BufferedInput.InputID, BindingEnum);

			const UWorld* World = AbilityComponent->GetWorld();
			if (World)
			{
				const float CurrentRealTime = World->GetRealTimeSeconds();
				ItemData.TimeLeft = DataPack.InputBufferTime - (CurrentRealTime - BufferedInput.BufferedTime);
			}

			DataPack.BufferedInputs.Add(ItemData);
		}
		
		const TArray<FGameplayAbilitySpec>& AbilitySpecs = AbilityComponent->GetActivatableAbilities();
		for (const FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
		{
			FRepData::FChaliceAbilityDebug ItemData;

			ItemData.Ability = GetNameSafe(AbilitySpec.Ability);
			ItemData.Ability.RemoveFromStart(DEFAULT_OBJECT_PREFIX);
			ItemData.Ability.RemoveFromEnd(TEXT("_C"));

			ItemData.Source = GetNameSafe(AbilitySpec.SourceObject);
			ItemData.Source.RemoveFromStart(DEFAULT_OBJECT_PREFIX);

			ItemData.Level = AbilitySpec.Level;
			ItemData.bIsActive = AbilitySpec.IsActive();

			if (AbilitySpec.Ability)
			{
				ItemData.bCanActivate = AbilitySpec.Ability->CanActivateAbility(AbilitySpec.Handle, AbilityComponent->AbilityActorInfo.Get());
			}
			ItemData.InputBinding = FRepData::FInputBindingDebug::CreateBinding(AbilitySpec.InputID, BindingEnum);

			DataPack.Abilities.Add(ItemData);
		}
	}
}

void FGameplayDebuggerCategory_ChaliceAbilities::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("Ability Subsystem"));
	
	CanvasContext.Printf(TEXT("\t{grey}Input buffer enabled: {yellow}%s"), DataPack.bInputBufferEnabled ? TEXT("True") : TEXT("False"));
	CanvasContext.Printf(TEXT("\t{grey}Input buffer time: {yellow}%.3f"), DataPack.InputBufferTime);
	CanvasContext.Printf(TEXT("\t{grey}Input buffer size: {yellow}%d"), DataPack.InputBufferSize);

	CanvasContext.Printf(TEXT("{grey}Buffered Inputs: {yellow}%d"), DataPack.BufferedInputs.Num());
	for (FRepData::FBufferedInputDebug& BufferedInput : DataPack.BufferedInputs)
	{
		const FVector& MovementInput = BufferedInput.MovementInput;
		CanvasContext.Printf(TEXT("\t%s {yellow}(%d)"), *BufferedInput.InputBinding.InputName, BufferedInput.InputBinding.InputID);
		CanvasContext.Printf(TEXT("\t\t{grey}Movement input: {yellow}(%.3f, %.3f, %.3f)"), MovementInput.X, MovementInput.Y, MovementInput.Z);
		CanvasContext.Printf(TEXT("\t\t{grey}Time remaining: {yellow}%.4f"), BufferedInput.TimeLeft);
	}

	CanvasContext.Printf(TEXT("Gameplay Abilities: {yellow}%d"), DataPack.Abilities.Num());
	for (const FRepData::FChaliceAbilityDebug& ItemData : DataPack.Abilities)
	{
		CanvasContext.Printf(TEXT("\t{yellow}%s {grey}source:{white}%s {grey}level:{white}%d {grey}active:{white}%s {grey}canactivate:{white}%s"),
            *ItemData.Ability, *ItemData.Source, ItemData.Level, ItemData.bIsActive ? TEXT("YES") : TEXT("no"), ItemData.bCanActivate ? TEXT("YES") : TEXT("no"));
		CanvasContext.Printf(TEXT("\t\t{grey}Binding: {yellow}%s(%d)"), *ItemData.InputBinding.InputName, ItemData.InputBinding.InputID);
	}
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_ChaliceAbilities::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_ChaliceAbilities());
}


// FRepData

void FGameplayDebuggerCategory_ChaliceAbilities::FRepData::Serialize(FArchive& Ar)
{
	Ar << bInputBufferEnabled;
	Ar << InputBufferTime;
	Ar << InputBufferSize;

	int32 BufferedInputsNum;
	if (Ar.IsSaving())
	{
		BufferedInputsNum = static_cast<int32>(BufferedInputs.Num());
	}
	Ar << BufferedInputsNum;
	if (Ar.IsLoading())
	{
		BufferedInputs.SetNumZeroed(BufferedInputsNum);
	}
	for (FBufferedInputDebug& BufferedInput : BufferedInputs)
	{
		Ar << BufferedInput.MovementInput;
		Ar << BufferedInput.InputBinding.InputID;
		Ar << BufferedInput.InputBinding.InputName;
		Ar << BufferedInput.TimeLeft;
	}

	int32 AbilitiesNum;
	if (Ar.IsSaving())
	{
		AbilitiesNum = static_cast<int32>(Abilities.Num());
	}
	Ar << AbilitiesNum;
	if (Ar.IsLoading())
	{
		Abilities.SetNumZeroed(AbilitiesNum);
	}
	for (FChaliceAbilityDebug& Ability : Abilities)
	{
		Ar << Ability.Ability;
		Ar << Ability.Source;
		Ar << Ability.Level;
		Ar << Ability.bIsActive;
		Ar << Ability.bCanActivate;
		Ar << Ability.InputBinding.InputID;
		Ar << Ability.InputBinding.InputName;
	}
}


// FInputBindingDebug factory method

FGameplayDebuggerCategory_ChaliceAbilities::FRepData::FInputBindingDebug
FGameplayDebuggerCategory_ChaliceAbilities::FRepData::FInputBindingDebug::CreateBinding(int32 InputID, const UEnum* InputBindEnum)
{
	FInputBindingDebug NewBinding;
	NewBinding.InputID = InputID;

	if (InputBindEnum)
	{
		NewBinding.InputName = InputBindEnum->GetNameStringByValue(InputID);
	}

	return NewBinding;
}

#endif
