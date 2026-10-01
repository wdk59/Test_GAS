// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatBarsWidget.generated.h"

class UResourceBarWidget;
class UPlayerAttributeSet;

/**
 * 
 */
UCLASS()
class TEST_GAS_API UPlayerStatBarsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializePlayerStatBars(UPlayerAttributeSet* InStat);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UResourceBarWidget> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UResourceBarWidget> ManaBar;
	
};
