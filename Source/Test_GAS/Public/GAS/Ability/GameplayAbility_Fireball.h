// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class TEST_GAS_API UGameplayAbility_Fireball : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGameplayAbility_Fireball();

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled
	) override;

	/** Checks cost. returns true if we can pay for the ability. False if not */
	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr	// OUT: 단순 표시용
	) const override;

private:

	// 스태미너 변경 시 실행될 콜백 함수
	void OnStaminaChanged(const FOnAttributeChangeData& InData);

	// AbilityTask_WaitInputPress용 콜백
	UFUNCTION()
	void OnWaitInputPressCallback(float InElapsedTime);

protected:

	// Effect: Burning Debuff (Infinite - Period)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|Effect|Debuff")
	TSubclassOf<UGameplayEffect> BurningDebuffEffectClass;

	// Effect: Mana Cost (Instant)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|Effect|Cost")
	TSubclassOf<UGameplayEffect> ManaCostEffectClass;

private:

	// Burning 디버프용 핸들
	FActiveGameplayEffectHandle DebuffEffectHandle;

	// Mana Cost용 핸들
	FActiveGameplayEffectHandle CostEffectHandle;

	// 마나 변경 감시용 델리게이트
	FDelegateHandle ManaChangedDelegateHandle;
	
};
