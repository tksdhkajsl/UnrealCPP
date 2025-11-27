// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/FloatingActor.h"
#include "FloatingActor_Curve.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API AFloatingActor_Curve : public AFloatingActor
{
	GENERATED_BODY()
	
protected:
	// 스테틱메시 움직임 업데이트하는 함수(기본적으로 커브를 사용하고 커브가 없으면 부모 코드 사용)
	virtual void OnFloatingMeshUpdate(float DeltaTime) override;

public:
	// 높이 움직임을 결정할 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> HeightCurve = nullptr;

	// 커브 한 주기 동안의 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 5.0f;
};
