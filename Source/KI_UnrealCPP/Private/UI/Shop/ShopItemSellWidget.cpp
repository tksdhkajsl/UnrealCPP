// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemSellWidget.h"
#include "UI/Inventory/InventoryDragDropOperation.h"
#include "Player/InventoryOwner.h"

bool UShopItemSellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp)
	{				
		if (invenOp->ItemData.IsValid())
		{
			int32 sellPrice = invenOp->Count * invenOp->ItemData->ItemPrice * 0.5f;	// 원래 가격의 절만만큼만 추가
			APawn* player = GetOwningPlayerPawn();
			if (player->Implements<UInventoryOwner>())
			{
				// 판매액만큼 돈을 추가
				IInventoryOwner::Execute_AddMoney(player, sellPrice);
				OnItemSell.Broadcast();
				return true;
			}
		}
	}

	return false;
}
