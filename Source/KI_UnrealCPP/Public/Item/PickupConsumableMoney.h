// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupConsumable.h"
#include "PickupConsumableMoney.generated.h"

/**
 * 파생 클래스 예시
 */
UCLASS()
class KI_UNREALCPP_API APickupConsumableMoney : public APickupConsumable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Money = 100;

protected:
	virtual void OnConsume() override;
};
