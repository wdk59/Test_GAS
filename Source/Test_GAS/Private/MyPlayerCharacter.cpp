// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Frameworks/MyHUD.h"
#include "AbilitySystemComponent.h"
#include "GAS/PlayerAttributeSet.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Stat"));

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
}

void AMyPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GiveFireballAbilities();

	// GAS 준비 후 HUD 연결
	if (IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(NewController))
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
			{
				MyHUD->InitHUD(this);
			}
		}
	}
}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyPlayerCharacter::GiveFireballAbilities()
{
	if (!ASC)
		return;
	
	// ASC 초기화 확인
	if (!ASC->AbilityActorInfo.IsValid())
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	if (FireballAbilityClass)
	{
		FGameplayAbilitySpec Spec(FireballAbilityClass, FireballAbilityLevel, FireballInputID);
		FireballAbilityHandle = ASC->GiveAbility(Spec);
	}
}

void AMyPlayerCharacter::OnFireInputStart()
{
	if (ASC)
	{
		ASC->AbilityLocalInputPressed(FireballInputID);
	}
}
