// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadHealthBarWidget.generated.h"

class UResourceBarWidget;

/**
 * 
 */
UCLASS()
class TEST_GAS_API UOverheadHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UResourceBarWidget> HealthBar = nullptr;

public:

	virtual void InitializeOverheadHealthBar(AActor* InActor);
	
};
