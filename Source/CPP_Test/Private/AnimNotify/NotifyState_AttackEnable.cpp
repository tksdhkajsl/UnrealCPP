// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/NotifyState_AttackEnable.h"
#include "Player/BP_ActionCharactor.h"

void UNotifyState_AttackEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);		
	
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<ABP_ActionCharactor>(MeshComp->GetOwner());
	}
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnAttackEnable(true);
	}
	
	UE_LOG(LogTemp,Log,TEXT(" NotifyState: %p"),this)
}

void UNotifyState_AttackEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnAttackEnable(false);
	}

}
