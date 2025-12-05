// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Usable/UsableItemDataAsset.h"
#include "UsableItemDataAsset_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UUsableItemDataAsset_HealthPotion : public UUsableItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData|HealthPotion")
	float HealAmount = 30.0f;


public:
	virtual void UseItem_Implementation(AActor* InTarget) override;
};
