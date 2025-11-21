// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_WeaponTrail.h"
#include "Player/BP_ActionCharactor.h"

void UAnimNotifyState_WeaponTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<ABP_ActionCharactor>(MeshComp->GetOwner());
	}

	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnWeaponTrailEnable(true); // 무기 이펙트 켜기
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotifyState_WeaponTrail::NotifyBegin : OwnerCharacter가 유효하지 않습니다."));
	}
}

void UAnimNotifyState_WeaponTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnWeaponTrailEnable(false); // 무기 이펙트 끄기
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotifyState_WeaponTrail::NotifyBegin : OwnerCharacter가 유효하지 않습니다."));
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
