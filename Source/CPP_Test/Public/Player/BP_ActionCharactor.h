// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BP_ActionCharactor.generated.h"

class UInputAction;
class USpringArmComponent;

UCLASS()
class CPP_TEST_API ABP_ActionCharactor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABP_ActionCharactor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//이동 방향 입력 받기
	void OnMoveInput(const FInputActionValue& InValue);
	//구르기 입력 받기
	void OnRollInput(const FInputActionValue& InValue);
	
	//달리기 모드 설정
	void SetSprintMode();
	//걷기 모드 설정
	void SetWalkingMode();

	/** 딜레이 시간이 지난 후 타이머가 호출할 함수 */
	void OnStaminaRegenDelayFinished();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<class UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float RollStamina = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float SprintStamina = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float RecoveryStamina = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float CurrentStamina = MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float StaminaRegenDelay;


	//구르기 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> RollMontage = nullptr;

	

	// 딜레이 타이머를 제어하기 위한 핸들
	FTimerHandle StaminaRegenTimerHandle;

private:

	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> AnimInstance = nullptr;

	bool IsSprinting;

	bool CanRegenStamina = true;


	

};
