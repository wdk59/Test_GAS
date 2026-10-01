// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/MyHUD.h"

#include "Widgets/MyHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.h"

void AMyHUD::InitHUD(APawn* InPawn)
{
	if (!InPawn || !HUDWidgetClass)
		return;

	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
	if (!PC)
		return;

	if (!HUDWidget)
	{
		HUDWidget = CreateWidget<UMyHUDWidget>(PC, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	// 폰의 GAS 정보에 맞춰서 HUDWidget 업데이트
	// -> 플레이어가 컨트롤러에 Possess될 때도 실행되도록 InitHUD() 호출하기
	if (HUDWidget)
	{
		HUDWidget->InitializeWithAbilitySystem(InPawn);
	}
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (APawn* OwningPawn = GetOwningPawn())
	{
		InitHUD(OwningPawn);
	}
}
