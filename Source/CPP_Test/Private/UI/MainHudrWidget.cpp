// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudrWidget.h"
#include "Player/BP_ActionCharactor.h"
#include "Player/ResourceComponent.h"
#include "UI/ResourceBarWidget.h"

void UMainHudrWidget::NativeConstruct()
{
	ABP_ActionCharactor* player = Cast<ABP_ActionCharactor>(GetOwningPlayerPawn());
	if (player && player->GetResourceComponent())
	{
		UResourceComponent* resource = player->GetResourceComponent();
		resource->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
		resource->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);

		HealthBar->RefreshWidget(resource->GetCurrentHealth(), resource->GetMaxHealth());
		StaminaBar->RefreshWidget(resource->GetCurrentStamina(), resource->GetMaxStamina());
	}
}
