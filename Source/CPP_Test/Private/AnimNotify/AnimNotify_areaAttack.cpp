// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_areaAttack.h"
#include "Player/BP_ActionCharactor.h"



void UAnimNotify_areaAttack::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<ABP_ActionCharactor>(MeshComp->GetOwner());
	}

	if (OwnerCharacter.IsValid())
	{
		// 범위 공격 할 타이밍이라고 알려만 주기
		OwnerCharacter->OnAreaAttack();
	}
}