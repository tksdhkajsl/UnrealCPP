// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupConsumableHealthOrb.h"
#include "Player/HasHealth.h"

void APickupConsumableHealthOrb::OnConsume()
{
	Super::OnConsume();	// APickupConsumable::OnConsume는 구현이 없다.

	if (PickupOwner.IsValid() && PickupOwner->Implements<UHasHealth>())
	{
		IHasHealth::Execute_HealHealth(PickupOwner.Get(), HealAmount);
	}
}
