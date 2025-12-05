// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Consumable/HealthOrbDataAsset.h"
#include "Player/HasHealth.h"

void UHealthOrbDataAsset::Excute(AActor* InTarget) const
{
	if (InTarget && InTarget->Implements<UHasHealth>())			// 대상이 체력이 있으면
	{
		IHasHealth::Execute_HealHealth(InTarget, HealAmount);	// 체력을 회복시켜라
	}
}
