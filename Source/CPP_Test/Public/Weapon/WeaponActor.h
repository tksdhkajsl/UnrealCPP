// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/BP_ActionCharactor.h"
#include "Common/CommonEnums.h"
#include "WeaponActor.generated.h"


UCLASS()
class CPP_TEST_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnWeaponActivate() {};
	virtual void OnWeaponDeactivate() {};

public:
	// 대상에게 데미지를 주는 함수
	UFUNCTION(BlueprintCallable)
	void DamageToTarget(AActor* InTarget);

	// 일정 범위 내의 적에게 데미지를 주는 함수
	UFUNCTION(BlueprintCallable)
	void DamageToArea();

	// 무기 자체를 활성화/비활성화 하는 함수(비지빌리티, 충돌 등등)
	UFUNCTION(BlueprintCallable)
	void WeaponActivate(bool bActivate);

	// 공격을 활성화/비활성화 하는 함수(컬리전 켜고 끄기)
	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);

	// 무기 트레일을 활성화/비활성화 하는 함수(나이아가라 켜고 끄기)
	UFUNCTION(BlueprintCallable)
	void TrailEnable(bool bEnable);

	// 공격을 했을 때 실행되어야 할 함수
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack() {};

	// 이 무기로 공격할 수 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanAttack() { return true; }

	// 무기를 획득했을 때 실행되는 함수
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped(int InCount);

	virtual void PostInitializeComponents() override;

	inline EItemCode GetWeaponID() const { return WeaponID; }

	inline void SetWeaponOwner(ABP_ActionCharactor* InOwner) { WeaponOwner = InOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> WeaponSlashEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemCode WeaponID = EItemCode::BasicWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float AreaInnerRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float AreaOuterRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float Falloff = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float DebugDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	TObjectPtr<class UNiagaraSystem> AreaAttackEffect = nullptr;

private:
	TWeakObjectPtr<ABP_ActionCharactor> WeaponOwner = nullptr;

};
