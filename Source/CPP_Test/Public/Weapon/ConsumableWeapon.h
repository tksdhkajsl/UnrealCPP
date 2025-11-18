// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "ConsumableWeapon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUseEnded);
/**
 * 
 */
UCLASS()
class CPP_TEST_API AConsumableWeapon : public AWeaponActor
{
	GENERATED_BODY()
	
public:
	virtual void OnAttack() override;
	virtual void OnWeaponPickuped(ABP_ActionCharactor* InOwner) override;

	virtual bool CanAttack() override { return RemainingUseCount > 0; }

protected:
	// 최대 사용회수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 MaxUseCount = 10;

	// 남은 사용회수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RemainingUseCount = 10;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Weapon")
	FOnWeaponUseEnded OnWeaponUseEnded;

};
