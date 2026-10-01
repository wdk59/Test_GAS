// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityCharacter.h"

#include "GAS/EnemyAttributeSet.h"

#include "MyEnemyCharacter.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class TEST_GAS_API AMyEnemyCharacter : public AMyAbilityCharacter
{
	GENERATED_BODY()

public :

	AMyEnemyCharacter();

	// Getter: AttributeSet
	inline UEnemyAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitializeOverheadWidget();

	/** 카메라 시선과 마주보도록(카메라 Forward의 반대 방향, 사이각 180도) 위젯 컴포넌트의 월드 회전을 갱신 */
	virtual void UpdateOverheadWidgetRotation();

protected :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UEnemyAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComp = nullptr;

	/** 위젯이 항상 카메라를 바라보도록 회전할지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bFaceCamera = true;

	/*
	* 빌보드 회전 시 상하 기울기(Pitch)를 고정(0)할지 여부
	* -> true면 수평 유지, false면 카메라 시선과 정확히 일치
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bLockWidgetPitch = false;

	/** 빌보드 회전 시 좌우 기울기(Roll)를 고정(0)할지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bLockWidgetRoll = true;
	
};
