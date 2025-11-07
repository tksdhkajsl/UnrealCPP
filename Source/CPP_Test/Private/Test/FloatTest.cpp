// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/FloatTest.h"

// Sets default values
AFloatTest::AFloatTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root")); // 씬 컴포넌트 하나 만들기
	SetRootComponent(root); //루트로 지정

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh")); // 스테틱 컴포넌트 하나 만들기
	BodyMesh->SetupAttachment(root); //root 아래에 붙이기
	

}

// Called when the game starts or when spawned
void AFloatTest::BeginPlay()
{
	Super::BeginPlay();
	
	//BodyMesh->SetRelativeLocation(FVector(0, 0, 100));
	//BodyMesh->AddRelativeLocation()
	
}

void AFloatTest::OnFloatingMeshUpdate(float DeltaTime)
{
	

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
/*if (CurrentLocation.Z > MoveHeight || CurrentLocation.Z < 0)
	{
		Speed *= -1.0f;
	}*/
	//bool IsMovingUp = true;

	//// 현재 상대 위치 가져오기
	//FVector CurrentLocation = BodyMesh->GetRelativeLocation();

	//if (IsMovingUp)
	//{
	//	// 1. 위로 이동 중일 때
	//	BodyMesh->AddRelativeLocation(DeltaTime * Speed * FVector::UpVector);

	//	// 2. 목표 높이(MoveHeight)에 도달했는지 확인
	//	if (CurrentLocation.Z > MoveHeight)
	//	{			
	//		IsMovingUp = false;
	//	}
	//}
	//else
	//{
	//	
	//	BodyMesh->AddRelativeLocation(DeltaTime * Speed * FVector::DownVector);

	//	
	//	if (CurrentLocation.Z < 0.0f) 
	//	{
	//		
	//		IsMovingUp = true;
	//	}
	//}
	//

// Called every frame
void AFloatTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnFloatingMeshUpdate(DeltaTime);
		
}

