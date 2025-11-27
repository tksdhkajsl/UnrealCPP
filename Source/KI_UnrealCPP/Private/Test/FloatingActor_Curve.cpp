// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/FloatingActor_Curve.h"

void AFloatingActor_Curve::OnFloatingMeshUpdate(float DeltaTime)
{
	if (HeightCurve)
	{
		// 커브가 있음
		ElapsedTime += DeltaTime;

		float curveInput = FMath::Fmod(ElapsedTime / Duration, 1.0f);	// Duration을 이용해 한주기 시간 결정, 0.0~1.0범위의 숫자로 반복
		float curveValue = HeightCurve->GetFloatValue(curveInput);		// 0~1~0

		//UE_LOG(LogTemp, Log, TEXT("Input : %.2f"), curveInput);
		//UE_LOG(LogTemp, Log, TEXT("Height : %.2f"), curveValue);
		BodyMesh->SetRelativeLocation(FVector(0, 0, curveValue * MoveHeight));
		BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
	}
	else
	{
		// 커브가 없음
		Super::OnFloatingMeshUpdate(DeltaTime);
	}
}
