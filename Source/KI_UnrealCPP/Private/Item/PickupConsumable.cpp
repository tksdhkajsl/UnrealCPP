// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupConsumable.h"

void APickupConsumable::OnPickupComplete_Implementation()
{
	OnConsume();

	Super::OnPickupComplete_Implementation();
}

void APickupConsumable::OnConsume()
{
	if (DataAsset)
	{
		DataAsset->Excute(PickupOwner.Get());
	}
}