// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityCharacter.h"

#include "GAS/PlayerAttributeSet.h"
#include "GameplayAbilitySpecHandle.h"

#include "MyPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGameplayAbility;

UCLASS()
class TEST_GAS_API AMyPlayerCharacter : public AMyAbilityCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

	// Getter: AttributeSet
	inline  UPlayerAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Fireball 입력 시작을 ASC에 전달
	// -> 컨트롤러에서 받아서 실행
	void OnFireInputStart();

protected:

	virtual void PossessedBy(AController* NewController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Fireball 어빌리티 부여
	void GiveFireballAbilities();

protected :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPlayerAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> FireballAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abillity", meta = (Config = "1"))
	int32 FireballAbilityLevel = 1;

private :

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle FireballAbilityHandle;

	static constexpr int32 FireballInputID = 100;

};
