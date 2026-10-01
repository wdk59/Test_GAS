// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ResourceBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UResourceBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetResourceBarColor(FillColor);
}

#if WITH_EDITOR
void UResourceBarWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티 이름 가져오기
	FName TargetPropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	// 변경된 프로퍼티 이름이 UResourceBarWidget의 FillColor인지 확인
	if (TargetPropertyName == GET_MEMBER_NAME_CHECKED(UResourceBarWidget, FillColor))
	{
		BackgroundColor = FillColor;
		BackgroundColor.A = BackgroundAlphaValue;
	}

	SetResourceBarColor(FillColor);
}
#endif

void UResourceBarWidget::UpdateResourceBar(float InCurrent, float InMax)
{
	if (!CurrentText || !MaxText)
		return;

	const float Percent = FMath::IsNearlyZero(InMax) ? 0.f : FMath::Clamp(InCurrent / InMax, 0.f, 1.f);
	Bar->SetPercent(Percent);
	CurrentText->SetText(FText::AsNumber(FMath::FloorToInt(InCurrent)));	// 소수점 떼기
	MaxText->SetText(FText::AsNumber(FMath::FloorToInt(InMax)));	// 소수점 떼기
}

void UResourceBarWidget::SetResourceBarColor(FLinearColor InColor)
{
	if (!Bar)
		return;

	FillColor = InColor;
	BackgroundColor = FillColor;
	BackgroundColor.A = BackgroundAlphaValue;

	Bar->SetFillColorAndOpacity(FillColor);	// 채워지는 색 변경

	FProgressBarStyle Style = Bar->GetWidgetStyle();	// 위젯 스타일 변경 위해서 기존 위젯 스타일 가져오기
	Style.BackgroundImage.TintColor = BackgroundColor;
	Bar->SetWidgetStyle(Style);
}
