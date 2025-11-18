// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ConsumableWeapon.h"

void AConsumableWeapon::OnAttack()
{
	RemainingUseCount--;
	if (RemainingUseCount <= 0)
	{
		OnWeaponUseEnded.Broadcast();
	}
}

void AConsumableWeapon::OnWeaponPickuped(ABP_ActionCharactor* InOwner)
{
	Super::OnWeaponPickuped(InOwner);

	RemainingUseCount = MaxUseCount;

}
