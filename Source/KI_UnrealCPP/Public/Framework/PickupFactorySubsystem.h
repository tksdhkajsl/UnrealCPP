// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Item/Pickup.h"
#include "PickupFactorySubsystem.generated.h"

/**
 * 픽업 아이템용 팩토리
 */
UCLASS()
class KI_UNREALCPP_API UPickupFactorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Pickup Factory")
	APickup* SpawnPickup(
		EItemCode InCode, FVector InLocation = FVector::ZeroVector, FRotator InRotator = FRotator::ZeroRotator);

private:
	void LoadPickupClassesMap();

	UPROPERTY()
	TMap<EItemCode, TSubclassOf<APickup>> PickupClasses;
};
