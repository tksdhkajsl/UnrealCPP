// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupConsumableMoney.h"
#include "Player/InventoryOwner.h"

void APickupConsumableMoney::OnConsume()
{
	Super::OnConsume();	// APickupConsumable::OnConsume는 구현이 없다.

	if (PickupOwner.IsValid() && PickupOwner->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddMoney(PickupOwner.Get(), Money);
	}
}
