// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPlayerCharacter.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
		return;

	// Input Mapping context 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Input Action 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnMoveInput);
		}

		// Look
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnLookInput);
		}

		// Jump
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::OnJumpInputStart);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::OnJumpInputEnd);
		}

		// Fire
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnFireInputStart);
		}
	}
}

void AMyPlayerController::OnMoveInput(const FInputActionValue & Value)
{
	// Vector2D 형식으로 입력
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		/* 앞 방향 찾기 */
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// 앞 방향 벡터
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 오른쪽 방향 벡터
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		/* 입력 방향으로 움직이기 */
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyPlayerController::OnLookInput(const FInputActionValue & Value)
{
	// Vector2D 형식으로 입력
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 시점 입력을 컨트롤러의 Yaw와 Pitch에 반영
	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void AMyPlayerController::OnJumpInputStart()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void AMyPlayerController::OnJumpInputEnd()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->StopJumping();
	}
}

void AMyPlayerController::OnFireInputStart()
{
	if (AMyPlayerCharacter* ControlledCharacter = Cast<AMyPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->OnFireInputStart();
	}
}
