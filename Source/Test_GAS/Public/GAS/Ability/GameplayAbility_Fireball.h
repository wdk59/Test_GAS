#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Fireball.generated.h"

class AFireballProjectile;

UCLASS()
class TEST_GAS_API UGameplayAbility_Fireball : public UGameplayAbility
{
    GENERATED_BODY()

public:

    UGameplayAbility_Fireball();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:

    // Fireball Projectile
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    TSubclassOf<AFireballProjectile> ProjectileClass;

    // Effect: Damage
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    // Effect: Burn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    TSubclassOf<UGameplayEffect> BurnEffectClass;

    // Projectile이 발사될 위치
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    FName FireSocket = TEXT("FireSocket");

};
