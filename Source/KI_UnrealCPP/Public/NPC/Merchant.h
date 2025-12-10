// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC/Interactable.h"
#include "Merchant.generated.h"

UCLASS()
class KI_UNREALCPP_API AMerchant : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerchant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnInteraction_Implementation() override;

	inline UDataTable* GetItemList() const { return ItemList; }

protected:
	UFUNCTION()
	void OnInteractionBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnInteractionEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> TextWidgetComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> InteractionBound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> LookCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Data")
	TObjectPtr<UDataTable> ItemList = nullptr;

private:
	
};
