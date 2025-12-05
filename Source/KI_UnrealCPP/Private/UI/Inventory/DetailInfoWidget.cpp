// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/DetailInfoWidget.h"
#include "Data/ItemDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UDetailInfoWidget::Open(UItemDataAsset* InItemData)
{
	SetInfo(InItemData);
	UpdateLocation();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDetailInfoWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
}

void UDetailInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateLocation();
}

void UDetailInfoWidget::SetInfo(UItemDataAsset* InItemData)
{
	if (InItemData)
	{
		ItemNameText->SetText(InItemData->ItemName);
		ItemDescriptionText->SetText(InItemData->ItemDescription);
		ItemPriceText->SetText(FText::AsNumber(InItemData->ItemPrice));
		ItemIconImage->SetBrushFromTexture(InItemData->ItemIcon);
	}
}

void UDetailInfoWidget::UpdateLocation()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	// SetPosition은 부모의 원점 기준 위치 설정
	CanvasSlot->SetPosition(MousePosition - ParentPosition);	// 그래서 부모의 위치를 빼서 뷰포트 기준의 위치로 변경
}
