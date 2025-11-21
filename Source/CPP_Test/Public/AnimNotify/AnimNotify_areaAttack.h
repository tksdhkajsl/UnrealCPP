// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_areaAttack.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TEST_API UAnimNotify_areaAttack : public UAnimNotify
{
	GENERATED_BODY()


public:
public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);

private:
	TWeakObjectPtr<class ABP_ActionCharactor> OwnerCharacter = nullptr;
};
