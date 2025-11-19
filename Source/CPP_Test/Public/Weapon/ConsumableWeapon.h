// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "Item/Consumable.h"
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
	virtual void OnWeaponPickuped(int InCount) override;

	virtual bool CanAttack() override { return RemainingUseCount > 0; }
	int32 GetRemainingUseCount() const { return RemainingUseCount; }

	//virtual FOnConsume& GetOnConsumeDelegate() { return OnWeaponUseEnded; };

protected:
	virtual void BeginPlay() override;
	virtual void OnWeaponActivate() override;

protected:
	// 최대 사용회수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 MaxUseCount = 10;

	// 남은 사용회수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RemainingUseCount = 10;

	// 무기를 다 사용했을 때 실행되는 델리게이트(현재 사용 안함)
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Weapon")
	FOnConsume OnWeaponUseEnded;
};
