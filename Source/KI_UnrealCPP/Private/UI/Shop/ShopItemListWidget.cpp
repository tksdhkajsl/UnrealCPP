// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemListWidget.h"
#include "UI/Shop/ShopItemBuyWidget.h"
#include "Data/DataTableRows.h"
#include "Components/VerticalBox.h"

void UShopItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemBuyContainer)
	{
		int count = ItemBuyContainer->GetChildrenCount();
		ShopItems.Empty(count);
		for (int32 i = 0; i < count; i++)
		{
			UShopItemBuyWidget* itemWidget = Cast<UShopItemBuyWidget>(ItemBuyContainer->GetChildAt(i));
			ShopItems.Add(itemWidget);
		}
	}
}

void UShopItemListWidget::ResetItemList(UDataTable* ItemList)
{
	// 전체 가중치 계산
	float totalWeight = 0.0f;
	TArray<FShopData_TableRow*> allRows;
	ItemList->GetAllRows(TEXT("ShopDataTable"), allRows);
	for (const FShopData_TableRow* row : allRows)
	{
		totalWeight += row->SelectWeight;
	}

	// 가중치에 따라 ShopItems갯수만큼 데이터 선택
	int32 selectCount = ShopItems.Num();
	TArray<const FShopData_TableRow*> selectedRows;
	selectedRows.Empty(selectCount);
	for (int32 i = 0; i < selectCount; i++)
	{
		float hit = FMath::FRandRange(0.0f, totalWeight);
		float currentWeight = 0.0f;
		for (const FShopData_TableRow* row : allRows)
		{
			currentWeight += row->SelectWeight;
			if (hit < currentWeight)
			{
				selectedRows.Add(row);
				break;
			}
		}
	}

	// 선택된 데이터에 따라 UI 갱신
	for (int32 i = 0; i < selectCount; i++)
	{
		const FShopData_TableRow* row = selectedRows[i];
		ShopItems[i]->InitializeItemBuy(row->ItemData, row->StockCount);
	}	
}

void UShopItemListWidget::UpdateAllBuyButton()
{
	for (const UShopItemBuyWidget* item : ShopItems)	// 자신이 가진 판매 아이템 목록들의 구매버턴 업데이트 지시
	{
		item->UpdateBuyButton();
	}
}
