// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Fireball.h"

#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	// 코스트와 쿨다운 검사 후, 가능하면 적용
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// Ability가 제대로 실행되지 않은 경우, End
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 마지막 true는 정상적인 종료가 아니라는 표시
		return;
	}

	// ASC 없으면 종료
	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 마지막 true는 정상적인 종료가 아니라는 표시
		return;
	}

	// Burning 디버프 적용
	if (BurningDebuffEffectClass)
	{
		FGameplayEffectSpecHandle DebuffSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, BurningDebuffEffectClass, GetAbilityLevel(Handle, ActorInfo));

		if (DebuffSpecHandle.IsValid())
		{
			DebuffEffectHandle = ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo, DebuffSpecHandle);
		}

		// Mana Cost 소모 이펙트 적용
		if (ManaCostEffectClass)
		{
			FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, SprintCostEffectClass, GetAbilityLevel(Handle, ActorInfo));

			if (CostSpecHandle.IsValid())
			{
				CostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CostSpecHandle);
			}
		}

	}
}

void UGameplayAbility_Fireball::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{}

bool UGameplayAbility_Fireball::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
	return false;
}

void UGameplayAbility_Fireball::OnStaminaChanged(const FOnAttributeChangeData& InData)
{}

void UGameplayAbility_Fireball::OnWaitInputPressCallback(float InElapsedTime)
{}
