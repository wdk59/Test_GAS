// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerStatBarsWidget.h"

#include "Widgets/ResourceBarWidget.h"
#include "GAS/PlayerAttributeSet.h"

void UPlayerStatBarsWidget::InitializePlayerStatBars(UPlayerAttributeSet* InStat)
{
	UE_LOG(LogTemp, Log, TEXT("Try Stat Update"));
	if (!InStat)
		return;

	UE_LOG(LogTemp, Log, TEXT("Stat Update Succeed"));
	InStat->OnHealthChange.AddUObject(HealthBar, &UResourceBarWidget::UpdateResourceBar);
	InStat->OnMaxHealthChange.AddUObject(HealthBar, &UResourceBarWidget::UpdateResourceBar);

	InStat->OnManaChange.AddUObject(ManaBar, &UResourceBarWidget::UpdateResourceBar);
	InStat->OnMaxManaChange.AddUObject(ManaBar, &UResourceBarWidget::UpdateResourceBar);

	HealthBar->UpdateResourceBar(InStat->GetHealth(), InStat->GetMaxHealth());
	ManaBar->UpdateResourceBar(InStat->GetMana(), InStat->GetMaxMana());

	UE_LOG(LogTemp, Log, TEXT("Health: %f, Mana: %f"), InStat->GetHealth(), InStat->GetMana());
}
