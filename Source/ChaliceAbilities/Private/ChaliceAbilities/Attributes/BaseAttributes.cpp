// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Attributes/BaseAttributes.h"
#include "GameplayEffectExtension.h"


// UBaseAttributes

// Attribute set overrides

void UBaseAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Limit max health to positive values
		NewValue = FMath::Max(0.f, NewValue);
		AdjustAttributeForMax(GetHealthAttribute(), GetMaxHealth(), NewValue);
	}
}

void UBaseAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageDone = GetDamage();
		SetDamage(0.f);

		if (DamageDone > 0.f)
		{
			SetHealth(FMath::Clamp(GetHealth() - DamageDone, 0.f, GetMaxHealth()));
		}
	}
}


// Helpers

void UBaseAttributes::AdjustAttributeForMax(const FGameplayAttribute& Attribute, float OldMax, float NewMax)
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	if (FMath::IsNearlyEqual(OldMax, NewMax) || !AbilityComponent)
	{
		return;
	}

	const float OldValue = Attribute.GetNumericValue(this);
	const float Alpha = OldMax > 0.f ? OldValue / OldMax : 1.f;
	const float NewValue = NewMax * Alpha;

	AbilityComponent->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Additive, NewValue - OldValue);
}

