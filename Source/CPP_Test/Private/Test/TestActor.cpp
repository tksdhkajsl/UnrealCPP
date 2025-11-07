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
	//super 부모클래스
	Super::BeginPlay(); //부모 클래스의 BeginPlay를 실행해라.

	/*UE_LOG(LogTemp, Log, TEXT("Hello Unreal. 한글도 됨."));
	UE_LOG(LogTemp, Warning, TEXT("Hello Unreal. 한글도 됨."));
	UE_LOG(LogTemp, Error, TEXT("Hello Unreal. 한글도 됨."));*/

	
}

void ATestActor::Test_NormalFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Test_NormalFunction 호출."));
}

void ATestActor::Test_UFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Test_UFunction 호출"));
}

//Test_NativeEventFunction구현은_Implementation 붙여야 한다.
void ATestActor::Test_NativeEventFunction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Test_NativeEventFunction_Implementation 호출"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Test_NativeEventFunction(); 다른 곳에서 호출할 때는 _Implementation 제외

}

