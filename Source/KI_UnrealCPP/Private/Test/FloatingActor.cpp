// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));	// 씬 컴포넌트 하나 만들기
	SetRootComponent(root);	// 루트로 지정

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));	// 스테틱 메시 컴포넌트 하나 만들기
	BodyMesh->SetupAttachment(root);	// root 아래에 붙이기
	
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloatingActor::OnFloatingMeshUpdate(float DeltaTime)
{
	//BodyMesh->AddRelativeLocation(DeltaTime * Speed * FVector::UpVector);

	//if (BodyMesh->GetRelativeLocation().Z > MoveHeight
	//	|| BodyMesh->GetRelativeLocation().Z < 0)
	//{
	//	Speed *= -1.0f;
	//}

	ElapsedTime += DeltaTime;	// 시간 누적시키기
	//UE_LOG(LogTemp, Log, TEXT("Elapsed Time : %.2f"), ElapsedTime);

	// Cos 함수 이용해서 위치기준 업데이트
	float cosValue = FMath::Cos(ElapsedTime);	// 1 -> -1 -> 1
	cosValue += 1;				// 2 -> 0 -> 2
	cosValue *= 0.5f;			// 1 -> 0 -> 1
	cosValue = 1 - cosValue;	// 0 -> 1 -> 0

	BodyMesh->SetRelativeLocation(FVector(0, 0, cosValue * MoveHeight));	// 위치 적용
	BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));	// 회전 적용
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnFloatingMeshUpdate(DeltaTime);
}

