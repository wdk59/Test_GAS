// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EnemyAttributeSet.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue)
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
}

void UEnemyAttributeSet::PostAttributeChange(const FGameplayAttribute & Attribute, float OldValue, float NewValue)
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
}

void UEnemyAttributeSet::AdjustAttributeForMaxChange(float InOldValue, float InNewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
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
