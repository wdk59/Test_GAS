#include "Projectiles/FireballProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/EnemyAttributeSet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFireballProjectile::AFireballProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetRelativeScale3D(FVector(0.35f));
    Mesh->SetSimulatePhysics(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetNotifyRigidBodyCollision(true);
    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->SetUpdatedComponent(Mesh);
    Movement->InitialSpeed = 1000.f;
    Movement->MaxSpeed = 1000.f;
    Movement->ProjectileGravityScale = 0.f;
    Movement->bRotationFollowsVelocity = true;
    Movement->bShouldBounce = false;
    InitialLifeSpan = 10.f;
}

void AFireballProjectile::InitializeEffects(
    UAbilitySystemComponent* Source,
    const FGameplayEffectSpecHandle& Damage,
    const FGameplayEffectSpecHandle& Burn)
{
    SourceASC = Source;
    DamageSpec = Damage;
    BurnSpec = Burn;
}

void AFireballProjectile::BeginPlay()
{
    Super::BeginPlay();

    OnActorHit.AddDynamic(this, &AFireballProjectile::OnHit);

    // 소유자 충돌 무시
    if (GetOwner())
    {
        Mesh->IgnoreActorWhenMoving(GetOwner(), true);
    }

    // 시전자 충돌 무시
    if (GetInstigator())
    {
        Mesh->IgnoreActorWhenMoving(GetInstigator(), true);
    }
}

void AFireballProjectile::OnHit(
    AActor* SelfActor,
    AActor* OtherActor,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    // 이미 충돌한 상태이거나 충돌 물체가 유효하지 않거나 소유자 또는 시전자인 경우 무시
    if (bHit
        || !OtherActor || OtherActor == this
        || OtherActor == GetOwner() || OtherActor == GetInstigator())
        return;

    bHit = true;

    // 피격자 ASC
    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
    
    if (SourceASC.IsValid() && TargetASC && TargetASC->GetSet<UEnemyAttributeSet>()
        && DamageSpec.IsValid() && BurnSpec.IsValid()
        && BurnStateTag.IsValid() && DamageMagnitudeTag.IsValid())
    {
        // Burn 적용 전에 기존 상태 판정
        const bool bAlreadyBurning = TargetASC->HasMatchingGameplayTag(BurnStateTag);

        /* Effect 적용: Damage */

        // 충돌 이펙트 스펙 복사: Damage
        FGameplayEffectSpec ImpactSpec(*DamageSpec.Data.Get());

        // 충돌 이펙트 스펙에 전달할 Context 복사: Damage
        FGameplayEffectContextHandle ImpactContext = ImpactSpec.GetContext().Duplicate();
        ImpactContext.AddHitResult(Hit, true);  // 복사한 Context에 충돌 정보 추가

        ImpactSpec.SetContext(ImpactContext);   // Spec에 Context 설정

        // 기존 Burn 상태에 따라 즉시 피해량 설정: 기본 10, Burn 중이면 20
        ImpactSpec.SetSetByCallerMagnitude(DamageMagnitudeTag, bAlreadyBurning ? 20.f : 10.f);

        // 피격자에게 충돌 이펙트 스펙 적용: Damage
        SourceASC->ApplyGameplayEffectSpecToTarget(ImpactSpec, TargetASC);

        /* Effect 적용 Burning */

        // 충돌 이펙트 스펙 복사: Burning
        FGameplayEffectSpec BurningSpec(*BurnSpec.Data.Get());

        // 충돌 이펙트 스펙에 전달할 Context 복사: Burning
        FGameplayEffectContextHandle BurnContext = BurningSpec.GetContext().Duplicate();
        BurnContext.AddHitResult(Hit, true);    // 복사한 Context에 충돌 정보 추가

        BurningSpec.SetContext(BurnContext);    // Spec에 Context 설정

        // 피격자에게 충돌 이펙트 스펙 적용: Damage
        SourceASC->ApplyGameplayEffectSpecToTarget(BurningSpec, TargetASC);
    }

    // 적 또는 벽 명중 시 한 번만 처리하고 소멸
    Destroy();
}
