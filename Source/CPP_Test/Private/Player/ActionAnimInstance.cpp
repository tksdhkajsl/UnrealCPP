// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"


void UActionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* ownerPawn = TryGetPawnOwner();
	if (ownerPawn)
	{
		OwnerMovementComponent = ownerPawn->GetMovementComponent();
	}
}

void UActionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerMovementComponent.IsValid())
	{
		//const UPawnMovementComponent* movement = OwnerMovementComponent.Get();
		//Speed = movement->Velocity.Size();
		Speed = OwnerMovementComponent.Get()->Velocity.Size();
		//UE_LOG(LogTemp, Log, TEXT("Speed : %.1f"), Speed);
	}
}
