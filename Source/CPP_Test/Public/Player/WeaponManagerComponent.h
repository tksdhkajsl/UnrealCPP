// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnums.h"
#include "Data/WeaponDataAsset.h"
#include "WeaponManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_TEST_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	AWeaponActor* GetEquippedWeapon(EItemCode InType) const;
	TSubclassOf<AUsedWeapon> GetUsedWeaponClass(EItemCode InType) const;
	TSubclassOf<APickup> GetPickupWeaponClass(EItemCode InType) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 시작시 데이터가 잘 들어있는지 체크해서 문제가 있으면 로그를 출력하는 함수
	void ValidateWeaponDatabase();

	// 시작시 손에 장비하는 무기 액터들을 모두 생성하는 함수
	void SpawnWeaponInstances();

public:

protected:
	// 이 컴포넌트가 관리하는 무기 클래스들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Database")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> WeaponDatabase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Instance")
	TMap<EItemCode, TObjectPtr<AWeaponActor>> WeaponInstances;

private:
	UPROPERTY()
	TWeakObjectPtr<class ABP_ActionCharactor> OwnerPlayer = nullptr;

		
};
