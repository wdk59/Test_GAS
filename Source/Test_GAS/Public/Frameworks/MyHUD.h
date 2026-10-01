// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMyHUDWidget;

/**
 * 
 */
UCLASS()
class TEST_GAS_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public :

	UFUNCTION(BlueprintCallable)
	void InitHUD(APawn* InPawn);

	UFUNCTION(BlueprintCallable)
	UMyHUDWidget* GetHUDWIdget() const { return HUDWidget; }

protected :

	virtual void BeginPlay() override;

protected :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMyHUDWidget> HUDWidget;

};
