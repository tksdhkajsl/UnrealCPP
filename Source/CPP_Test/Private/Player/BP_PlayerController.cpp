// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"



void ABP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);
	if (enhanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABP_PlayerController::OnLookInput);
	}
}

void ABP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());	// Subsystem 가져오기 시도
	if (Subsystem)	// Subsystem이 null이 아니면
	{
		//UE_LOG(LogTemp, Log, TEXT("바인드 성공"));
		Subsystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}
	PlayerCameraManager->ViewPitchMax = ViewPitchMax; //카메라 위아래 최대치 설정
	PlayerCameraManager->ViewPitchMin = ViewPitchMin; //카메라 위아래 최소치 설정
}

void ABP_PlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D lookAxis = InValue.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("onlookinput: %s"),lookAxis.ToString());
	AddYawInput(lookAxis.X);
	AddPitchInput(lookAxis.Y);
}
