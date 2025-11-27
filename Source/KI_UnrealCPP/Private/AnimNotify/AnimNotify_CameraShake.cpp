// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_CameraShake.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	//UE_LOG(LogTemp, Log, TEXT("UAnimNotify_CameraShake::Notify"));

	if (!CameraManager.IsValid())	// CameraManager가 없으면 미리 찾아 놓기
	{
		UWorld* world = MeshComp->GetWorld();
		if (world && world->GetFirstPlayerController())
		{
			CameraManager = world->GetFirstPlayerController()->PlayerCameraManager;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("UAnimNotify_CameraShake::Notify : CameraShake(%p), CameraManager(%p)"), CameraShake.Get(), CameraManager.Get());

	if (CameraShake && CameraManager.IsValid())	// class랑 매니저 둘 다 있을 때만 실행
	{
		//UE_LOG(LogTemp, Log, TEXT("UAnimNotify_CameraShake::Notify - 카메라 흔들기 실행"));
		CameraManager->StartCameraShake(CameraShake);
	}

	//UE_LOG(LogTemp, Log, TEXT("UAnimNotify_CameraShake::Notify - 종료"));
}
