// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EnemyAttributeSet.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitDamage(0.f);
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

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.f);	// Damage 즉시 비우기

		if (LocalDamage > 0)
		{
			float FinalDamage = LocalDamage;	// 방어력 없으므로 Damage 그대로 받음
			FinalDamage = FMath::Max(1.f, FinalDamage);	// 맞으면 최소 1

			const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.f, GetMaxHealth());
			SetHealth(NewHealth);
		}
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
