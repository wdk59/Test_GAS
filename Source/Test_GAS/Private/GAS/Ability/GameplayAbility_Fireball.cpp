#include "GAS/Ability/GameplayAbility_Fireball.h"
#include "Projectiles/FireballProjectile.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Fireball::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    ACharacter* Character =
        ActorInfo
        ? Cast<ACharacter>(ActorInfo->AvatarActor.Get())
        : nullptr;

    UAbilitySystemComponent* SourceASC =
        ActorInfo
        ? ActorInfo->AbilitySystemComponent.Get()
        : nullptr;

    // Ability 발동에 필요한 게 하나라도 없으면 Ability 취소
    if (!Character || !SourceASC || !ProjectileClass
        || !DamageEffectClass || !BurnEffectClass
        || !GetCostGameplayEffect()
        || !GetCooldownGameplayEffect())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    const float Level = GetAbilityLevel(Handle, ActorInfo);

    FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, Level);
    FGameplayEffectSpecHandle BurnSpec = MakeOutgoingGameplayEffectSpec(BurnEffectClass, Level);

    // 이펙트 Spec 생성 실패 또는 비용/쿨타임 조건 불충족 시 Ability 취소
    if (!DamageSpec.IsValid() || !BurnSpec.IsValid()
        || !CommitCheck(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 발사할 Transform 정보 저장
    const FRotator Rotation = Character->GetActorRotation();
    const FVector Location =
        Character->GetMesh()->DoesSocketExist(FireSocket)
        ? Character->GetMesh()->GetSocketLocation(FireSocket)
        : Character->GetActorLocation() + Character->GetActorForwardVector();
    const FTransform Transform(Rotation, Location);

    /* 발사체  스폰 */
    // 일반 SpawnActor()는 반환되기 전에 BeginPlay()가 실행되기 때문에
    // InitializeEffects() 호출 전에 발사체의 충돌 처리가 시작될 수 있음
    // -> 지연 스폰으로 구현

    // 발사체 스폰 지연 시작: 생성 완료 전에 필요한 데이터 설정
    AFireballProjectile* Projectile = Character->GetWorld()->SpawnActorDeferred<AFireballProjectile>(
        ProjectileClass,
        Transform,
        Character,
        Character,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    // 발사체 생성 실패 시 Ability 취소
    if (!Projectile)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 발사체 명중 시 적용할 이펙트 Spec과 시전자 ASC 전달
    Projectile->InitializeEffects(SourceASC, DamageSpec, BurnSpec);

    // 비용/쿨타임 커밋 실패 시 발사체 제거 후 Ability 취소
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        Projectile->Destroy();
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 발사체 스폰 완료
    Projectile->FinishSpawning(Transform);

    // 발사 완료 후 Ability 정상 종료
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
