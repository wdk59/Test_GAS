// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class TEST_GAS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected :

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// Move 입력 처리
	void OnMoveInput(const FInputActionValue& Value);

	// Look 입력 처리
	void OnLookInput(const FInputActionValue& Value);

	// Jump 입력 처리
	void OnJumpInputStart();
	void OnJumpInputEnd();
	
	// Fire 입력 처리
	void OnFireInputStart();

public:

	// Input Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	// Fire Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Ability")
	TObjectPtr<UInputAction> FireAction;
};
