// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Pickup.h"
#include "Data/Consumable/ConsumableDataAsset.h"
#include "PickupConsumable.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API APickupConsumable : public APickup
{
	GENERATED_BODY()

public:
	virtual void OnPickupComplete_Implementation() override;

protected:
	virtual void OnConsume();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UConsumableDataAsset> DataAsset = nullptr;
	
};
