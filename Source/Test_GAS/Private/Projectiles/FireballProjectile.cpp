// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/FireballProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AFireballProjectile::AFireballProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetRelativeScale3D(FVector(0.35f));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1000.f;
	Movement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AFireballProjectile::OnHit);

	if (GetInstigator())
	{
		Mesh->IgnoreActorWhenMoving(GetInstigator(), true);	// 인스티게이터는 충돌 무시
	}

	SetLifeSpan(FireballLifeSpan);	// 스폰 10초 뒤 삭제
}

void AFireballProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		// 이전에 부딪힌 적이 없고, 다른 액터가 캐릭터여야 하고, 다른 액터가 내가 아니고, 다른 액터가 오너도 아니다.
		if (!bHit && OtherActor->IsA<ACharacter>() && OtherActor != this && GetOwner() != OtherActor)
		{
			bHit = true;

			/* GameplayEffect으로 바꿔야 함 */
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

			SpawnHitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
}

void AFireballProjectile::SpawnHitEffect(const FVector& InLocation, const FRotator& InRotator)
{
	if (HitVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, InLocation, InRotator);
	}
}
