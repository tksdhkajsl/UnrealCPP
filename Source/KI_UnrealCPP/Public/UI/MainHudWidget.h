// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/Shop/ShopWidget.h"
#include "MainHudWidget.generated.h"

UENUM(BlueprintType)
enum class EOpenState : uint8
{
	Open	UMETA(DisplayName = "Open"),
	Close	UMETA(DisplayName = "Close")
};
/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void CloseInventory();

	UFUNCTION(BlueprintCallable, Category = "UI|Shop")
	void OpenShop(UDataTable* ItemList);

	UFUNCTION(BlueprintCallable, Category = "UI|Shop")
	void CloseShop();

	void AddToInventoryCloseDelegate(const FScriptDelegate& Delegate)
	{
		if (Inventory)
		{
			Inventory->OnInventoryCloseRequested.Add(Delegate);
		}
	}

	void AddToShopCloseDelegate(const FScriptDelegate& Delegate)
	{
		if (Shop)
		{
			Shop->OnShopCloseRequested.Add(Delegate);
		}
	}
	
	inline EOpenState GetInventoryState() const { return InventoryState; }	
	inline UInventoryWidget* GetInventoryWidget() const { return Inventory; }

	inline bool IsInventoryOpen() const { return InventoryState == EOpenState::Open; }
	inline bool IsShopOpen() const { return ShopState == EOpenState::Open; }

protected:
	// meta = (BindWidget)
	// 위젯 블루프린트의 변수와 이 클래스의 변수를 바인드하겠다라는 의미(=둘이 같은 거라고 설정)
	// 위젯 블루프린트의 변수명과 이 클래스의 변수명이 반드시 같아야 한다.(대소문자도 같아야 한다)
	UPROPERTY(BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> StaminaBar = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UInventoryWidget> Inventory = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop", meta = (BindWidget))
	TObjectPtr<class UShopWidget> Shop = nullptr;

private:
	EOpenState InventoryState = EOpenState::Close;
	EOpenState ShopState = EOpenState::Close;
	
};
