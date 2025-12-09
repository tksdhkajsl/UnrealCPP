// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemBuyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Data/ItemDataAsset.h"
#include "Player/InventoryOwner.h"

void UShopItemBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemCount)
	{
		ItemCount->SetHintText(FText::AsNumber(MinimumBuyCount));
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextChanged);		// 변경이 있을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextCommitted);	// 변경을 확정했을 때(엔터친 후, 포커스를 잃은 후)
	}

	if (ItemBuy)
	{
		ItemBuy->OnClicked.AddDynamic(this, &UShopItemBuyWidget::OnBuyButtonClicked);
	}

	if (SoldOut)
	{
		SoldOut->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UShopItemBuyWidget::InitializeItemBuy(UItemDataAsset* InItemData, int32 InStockCount)
{
	ItemCount->SetIsEnabled(true);
	SoldOut->SetVisibility(ESlateVisibility::Hidden);

	ItemIcon->SetBrushFromTexture(InItemData->ItemIcon);
	ItemName->SetText(InItemData->ItemName);
	ItemPrice->SetText(FText::AsNumber(InItemData->ItemPrice));
	ItemDescription->SetText(InItemData->ItemDescription);

	ItemData = InItemData;
	SetStockCount(InStockCount);
	SetBuyCount(MinimumBuyCount);

	UpdateBuyButton();
}

void UShopItemBuyWidget::OnItemCountTextChanged(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Changed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{		
		SetBuyCount(FMath::Clamp(FCString::Atoi(*number), MinimumBuyCount, StockCount));	// 갯수는 1~StockCount 사이
		UpdateBuyButton();
	}
}

void UShopItemBuyWidget::OnItemCountTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("Committed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		SetBuyCount(FCString::Atoi(*number));
	}
	else
	{
		SetBuyCount(MinimumBuyCount);
	}
}

void UShopItemBuyWidget::OnBuyButtonClicked()
{
	//UE_LOG(LogTemp, Log, TEXT("구매 버튼 클릭"));
	
	APawn* player = GetOwningPlayerPawn();
	if (player->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(player, ItemData.Get(), BuyCount);
		IInventoryOwner::Execute_RemoveMoney(player, ItemData->ItemPrice * BuyCount);
		//StockCount -= BuyCount;
		SetStockCount(StockCount - BuyCount);
		SetBuyCount(MinimumBuyCount);
		if (StockCount < MinimumBuyCount)
		{
			// 갯수 입력 금지
			ItemCount->SetIsEnabled(false);

			// 매진 표시
			SoldOut->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

}

void UShopItemBuyWidget::SetStockCount(int32 InCount)
{
	StockCount = InCount;
	ItemStockCount->SetText(FText::AsNumber(StockCount));
	UpdateBuyButton();
}

void UShopItemBuyWidget::SetBuyCount(int32 InCount)
{
	BuyCount = InCount;
	if (BuyCount > MinimumBuyCount)
	{
		ItemCount->SetText(FText::AsNumber(BuyCount));
	}
	else
	{
		ItemCount->SetText(FText::GetEmpty());
	}
}

void UShopItemBuyWidget::UpdateBuyButton() const
{
	if (StockCount < MinimumBuyCount)
	{
		ItemBuy->SetIsEnabled(false);
	}
	else
	{
		APawn* player = GetOwningPlayerPawn();
		if (player && player->Implements<UInventoryOwner>())
		{
			// 플레이어의 돈 상태 확인해서 버튼 활성화/비활성화
			bool hasEnouhgMoney = IInventoryOwner::Execute_HasEnoughMoney(player, BuyCount * ItemData->ItemPrice);
			ItemBuy->SetIsEnabled(hasEnouhgMoney);
		}
	}
}
