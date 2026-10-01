// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityCharacter.h"

#include "GAS/EnemyAttributeSet.h"

#include "MyEnemyCharacter.generated.h"

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

protected :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UEnemyAttributeSet> AttributeSet;
	
};
