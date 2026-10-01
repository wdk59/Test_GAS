#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "FireballProjectile.generated.h"

class UStaticMeshComponent;
class UAbilitySystemComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;

UCLASS()
class TEST_GAS_API AFireballProjectile : public AActor
{
    GENERATED_BODY()

public:

    AFireballProjectile();

    void InitializeEffects(
        UAbilitySystemComponent* Source,
        const FGameplayEffectSpecHandle& Damage,
        const FGameplayEffectSpecHandle& Burn);

protected:

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(
        AActor* SelfActor,
        AActor* OtherActor,
        FVector NormalImpulse,
        const FHitResult& Hit);
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fireball")
    TObjectPtr<UStaticMeshComponent> Mesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fireball")
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fireball|VFX")
    TObjectPtr<UNiagaraComponent> FireballVFX;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    FGameplayTag BurnStateTag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fireball")
    FGameplayTag DamageMagnitudeTag;

private:

    TWeakObjectPtr<UAbilitySystemComponent> SourceASC;

    FGameplayEffectSpecHandle DamageSpec;

    FGameplayEffectSpecHandle BurnSpec;

    bool bHit = false;  // 발사체 충돌 여부
};
