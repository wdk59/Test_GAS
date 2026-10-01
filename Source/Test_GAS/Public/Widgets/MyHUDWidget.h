// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUDWidget.generated.h"

class UPlayerStatBarsWidget;

/**
 * 
 */
UCLASS()
class TEST_GAS_API UMyHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void InitializeWithAbilitySystem(APawn* InPawn);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPlayerStatBarsWidget> StatWidget;
	
};
