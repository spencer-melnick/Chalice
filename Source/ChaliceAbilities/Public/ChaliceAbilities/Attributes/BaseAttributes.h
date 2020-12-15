// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributes.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * Attribute set with common character attributes like health, damage, stamina, etc.
 */
UCLASS()
class CHALICEABILITIES_API UBaseAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:

	// Attribute set overrides

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;


	// Attributes

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributes, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributes, Health)

	UPROPERTY(BlueprintReadOnly, Category="Defense")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UBaseAttributes, Defense)

	UPROPERTY(BlueprintReadOnly, Category="Damage", meta=(HiddenFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseAttributes, Damage)


protected:

	// Helpers

	void AdjustAttributeForMax(const FGameplayAttribute& Attribute, float OldMax, float NewMax);

};
