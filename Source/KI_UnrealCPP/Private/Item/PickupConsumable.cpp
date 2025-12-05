// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupConsumable.h"

void APickupConsumable::OnPickupComplete_Implementation()
{
	OnConsume();	// 기능 동작

	Super::OnPickupComplete_Implementation();	// 자기 자신 삭제
}

void APickupConsumable::OnConsume()
{
	if (DataAsset)
	{
		DataAsset->Excute(PickupOwner.Get());
	}
}