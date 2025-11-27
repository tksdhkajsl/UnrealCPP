// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Consumable/ConsumableDataAsset.h"
#include "HealthOrbDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UHealthOrbDataAsset : public UConsumableDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthOrb")
	float HealAmount = 30.0f;

	virtual void Excute(AActor* InTarget) const override;
};
