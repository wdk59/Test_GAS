// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverheadHealthBarWidget.h"

#include "Widgets/ResourceBarWidget.h"
#include "MyEnemyCharacter.h"
#include "GAS/EnemyAttributeSet.h"

void UOverheadHealthBarWidget::InitializeOverheadHealthBar(AActor* InActor)
{
	if (AMyEnemyCharacter* Character = Cast<AMyEnemyCharacter>(InActor))
	{
		if (UEnemyAttributeSet* Stat = Character->GetAttributeSet())	// Stat Attribute Set 사용하는지 확인용
		{
			UE_LOG(LogTemp, Log, TEXT("Character has Stat"));
			Stat->OnHealthChange.AddUObject(HealthBar, &UResourceBarWidget::UpdateResourceBar);
			Stat->OnMaxHealthChange.AddUObject(HealthBar, &UResourceBarWidget::UpdateResourceBar);

			HealthBar->UpdateResourceBar(Stat->GetHealth(), Stat->GetMaxHealth());
		}
	}
}
