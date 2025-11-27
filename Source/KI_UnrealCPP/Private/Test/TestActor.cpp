// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	// Super = 부모 클래스
	Super::BeginPlay();	// 부모 클래스의 BeginPlay를 실행해라.
	
	UE_LOG(LogTemp, Log, TEXT("Hello Unreal. 한글도 됨. 일반 로그."));
	UE_LOG(LogTemp, Warning, TEXT("Hello Unreal. 한글도 됨. 경고 로그."));
	UE_LOG(LogTemp, Error, TEXT("Hello Unreal. 한글도 됨. 에러 로그."));
}

void ATestActor::Test_NormalFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Test_NormalFunction 호출"));
}

void ATestActor::Test_UFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Test_UFunction 호출"));
}

// Test_NativeEventFunction의 구현은 _Implementation를 붙여야 한다.
void ATestActor::Test_NativeEventFunction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Test_NativeEventFunction 호출"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Test_NativeEventFunction(); C++에서 호출은 _Implementation 안붙인 이름 사용
}

