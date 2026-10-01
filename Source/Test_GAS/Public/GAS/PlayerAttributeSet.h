// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChange, float, float);
UCLASS()
class TEST_GAS_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public :

	UPlayerAttributeSet();

	// 적용 전 Clamping 수행
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// 적용 후 값의 변화 감지나 UI에 반영하기 위해 사용
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// 이펙트가 적용된 후에 실행되는 함수
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:

	void AdjustAttributeForMaxChange(float InOldValue, float InNewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

public :

	/* Stat 변경 델리게이트 */

	FOnStatChange OnHealthChange;

	FOnStatChange OnMaxHealthChange;

	FOnStatChange OnManaChange;

	FOnStatChange OnMaxManaChange;

	/* 타입 만들기 */

	// Health 타입
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Health);

	// MaxHealth 타입
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxHealth);

	// Mana 타입
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Mana);

	// MaxMana 타입
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxMana);

	// ManaCost 타입
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData ManaCost;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, ManaCost);
};
