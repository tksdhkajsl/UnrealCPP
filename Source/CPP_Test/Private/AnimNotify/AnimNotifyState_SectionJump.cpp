// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Player/BP_ActionCharactor.h"

void UAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharactor = Cast<ABP_ActionCharactor>(MeshComp->GetOwner());
	if (OwnerCharactor.IsValid())
	{
		//OwnerCharactor에 자기자신을 설정
		OwnerCharactor->SetSectionJumpNotify(this);
	}
}

void UAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharactor.IsValid())
	{
		//OwnerCharactor 설정 해제
		OwnerCharactor->SetSectionJumpNotify(nullptr);
		OwnerCharactor = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
