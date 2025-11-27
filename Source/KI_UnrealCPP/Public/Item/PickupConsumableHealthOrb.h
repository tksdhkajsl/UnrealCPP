// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupConsumable.h"
#include "PickupConsumableHealthOrb.generated.h"

/**
 * 파생 클래스 예시
 */
UCLASS()
class KI_UNREALCPP_API APickupConsumableHealthOrb : public APickupConsumable
{
	GENERATED_BODY()	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealOrb")
	float HealAmount = 30.0f;

protected:
	virtual void OnConsume() override;
};
