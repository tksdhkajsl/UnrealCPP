// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/TemporarySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTemporarySlotWidget::SetItemIconImage(UTexture2D* InTexture)
{
	ItemIconImage->SetBrushFromTexture(InTexture);	
}

void UTemporarySlotWidget::SetCountText(int32 InCount)
{
	CountText->SetText(FText::AsNumber(InCount));
}
