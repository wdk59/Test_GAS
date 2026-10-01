// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitMana(100.f);
	InitMaxMana(100.f);

	InitManaCost(0.f);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0, NewValue);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(0, NewValue);
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute & Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		OnHealthChange.Broadcast(NewValue, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(OldValue, NewValue, GetHealthAttribute());

		OnMaxHealthChange.Broadcast(GetHealth(), NewValue);
	}
	else if (Attribute == GetManaAttribute())
	{
		OnManaChange.Broadcast(NewValue, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(OldValue, NewValue, GetMaxManaAttribute());

		OnMaxManaChange.Broadcast(GetMana(), NewValue);
	}
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaCostAttribute())
	{
		const float LocalCost = GetManaCost();
		SetManaCost(0.f);;	// Cost 즉시 비우기

		if (LocalCost > 0)
		{
			float FinalCost = LocalCost;
			FinalCost = FMath::Max(0.f, FinalCost);	// 0 이하는 안됨

			const float NewMana = FMath::Clamp(GetMana() - FinalCost, 0.f, GetMaxMana());
			SetMana(NewMana);
		}
	}
}

void UPlayerAttributeSet::AdjustAttributeForMaxChange(float InOldValue, float InNewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
		return;

	float Delta = InNewMaxValue - InOldValue;

	if (Delta > 0)
	{
		ASC->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, Delta);
	}
	else
	{
		bool bFound = false;
		const float Current = ASC->GetGameplayAttributeValue(AffectedAttributeProperty, bFound);
		AffectedAttributeProperty.GetGameplayAttributeData(this);
		if (InNewMaxValue < Current)
		{
			Delta = InNewMaxValue - Current;
			ASC->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, Delta);
		}
	}
}
