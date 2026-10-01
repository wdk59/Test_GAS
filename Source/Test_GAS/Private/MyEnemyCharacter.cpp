// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyCharacter.h"

#include "GAS/EnemyAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Widgets/OverheadHealthBarWidget.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

AMyEnemyCharacter::AMyEnemyCharacter()
{
	AttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("Stat"));

	// 카메라를 바라보는 빌보드 회전 처리를 위해 틱 활성화
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComp->SetupAttachment(RootComponent);

	HealthBarWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComp->SetDrawSize(FVector2D(750.f, 50.f));
	HealthBarWidgetComp->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));
	HealthBarWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthBarWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);	// 타이밍 문제로 추가 처리
	}

	InitializeOverheadWidget();
}

void AMyEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFaceCamera)
	{
		UpdateOverheadWidgetRotation();
	}
}

void AMyEnemyCharacter::InitializeOverheadWidget()
{
	if (!HealthBarWidgetComp)
		return;

	if (UUserWidget* UserWidget = HealthBarWidgetComp->GetUserWidgetObject())
	{
		if (UOverheadHealthBarWidget* HealthBarWidget = Cast<UOverheadHealthBarWidget>(UserWidget))
		{
			HealthBarWidget->InitializeOverheadHealthBar(this);
		}
	}
}

void AMyEnemyCharacter::UpdateOverheadWidgetRotation()
{
	if (!HealthBarWidgetComp)
		return;

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		// 카메라의 전방 벡터와 정확히 마주보는 방향(-CameraForward, 사이각 180도)으로 회전
		const FVector CameraForward = CameraManager->GetCameraRotation().Vector();
		FRotator WidgetRotation = (-CameraForward).Rotation();

		if (bLockWidgetPitch)
		{
			WidgetRotation.Pitch = 0.f;
		}
		if (bLockWidgetRoll)
		{
			WidgetRotation.Roll = 0.f;
		}

		HealthBarWidgetComp->SetWorldRotation(WidgetRotation);
	}
}
