// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"
#include "Player/InventoryComponent.h"
#include "UI/ResourceBarWidget.h"
#include "UI/Shop/ShopWidget.h"

void UMainHudWidget::NativeConstruct()
{
	CloseInventory();
	CloseShop();

	AActionCharacter* player = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if (player)
	{
		if (UResourceComponent* resource = player->GetResourceComponent())
		{
			// 리소스 컴포넌트에서 HP와 스테미너의 변화가 있을 때 UI 업데이트
			resource->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);	// 예시용
			resource->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);		

			HealthBar->RefreshWidget(resource->GetCurrentHealth(), resource->GetMaxHealth());
			StaminaBar->RefreshWidget(resource->GetCurrentStamina(), resource->GetMaxStamina());
		}

		if (UInventoryComponent* inventoryComponent = player->GetInventoryComponent())
		{
			if (Shop)
			{
				// 인벤토리 컴포넌트에서 돈의 변화가 있으면 상점 아이템 리스트의 구매버튼 상태를 변경한다.
				inventoryComponent->OnInventoryMoneyChanged.AddDynamic(Shop, &UShopWidget::UpdateAllBuyButtonState);
			}
		}
	}

	if (Inventory && Shop)
	{		
		// 상점위젯에서 아이템 판매가 일어났으면 인벤토리 위젯의 상세정보창 닫기
		FScriptDelegate sellDelegate;
		sellDelegate.BindUFunction(Inventory, "CloseDetailInfo");
		Shop->AddToItemSellDelegate(sellDelegate);
	}
}

void UMainHudWidget::OpenInventory()
{
	Inventory->RefreshInventoryWidget();	// 열릴때마다 UI 내용 갱신
	Inventory->SetVisibility(ESlateVisibility::Visible);
	InventoryState = EOpenState::Open;
}

void UMainHudWidget::CloseInventory()
{
	InventoryState = EOpenState::Close;
	Inventory->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHudWidget::OpenShop(UDataTable* ItemList)
{
	if (ItemList)
	{
		Shop->InitializeShop(ItemList);
		Shop->SetVisibility(ESlateVisibility::Visible);
		ShopState = EOpenState::Open;
	}
}

void UMainHudWidget::CloseShop()
{
	ShopState = EOpenState::Close;
	Shop->SetVisibility(ESlateVisibility::Hidden);
}
