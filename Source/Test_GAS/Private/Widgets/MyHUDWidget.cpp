// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MyHUDWidget.h"
#include "Widgets/PlayerStatBarsWidget.h"
#include "MyPlayerCharacter.h"

void UMyHUDWidget::InitializeWithAbilitySystem(APawn* InPawn)
{
	if (!InPawn)
		return;

	AMyPlayerCharacter* Player = Cast<AMyPlayerCharacter>(InPawn);
	if (!Player)
		return;

	if (StatWidget)
	{
		StatWidget->InitializePlayerStatBars(Player->GetAttributeSet());
	}
}
