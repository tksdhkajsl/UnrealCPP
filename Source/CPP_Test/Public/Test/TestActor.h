// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//generated.h는 모든 헤더 중 마지막에 있어야 한다.
#include "TestActor.generated.h"

//UCLASS()
// C++ 클래스를 언리얼 엔진의 객체 시스템에 등록(=UObject)
// 이 매크로가 붙은 클래스만이 언리얼 엔진의 객체로 인식되고 관리된다.
/*
	주요기능
		- 가비지 콜렉팅
		- 블루프린트 연동
		- 에디터 노출
*/
//UFUNCTION
//	블루프린트에서 함수를 호출, 구현 ,재정의 등 할때 UFUNCTION사용
//  다이나믹 델리게이트에 바인딩 할 경우 UFUNCTION 필수
//	블루프린트에서 사용될 경우에 반드시 필요하다.

//UPROPERTY()
/*
	UCLASS나 USTRUCT내의 맵버 변수를 언리얼 엔진의 리플렉션 시스템에 등록
	-주요기능
		- 에디터에 노출 및 편집을 가능하게 한다.
		- 블루프린터 접근
		- 가비지 컬렉션이 추적을 가능하게 함.
		- 직렬화(변수의 값이 레벨이나 에셋에 함께 저장)
		- 네트워크 복제(Replicated)
*/
UCLASS()
class CPP_TEST_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test_NormalFunction();

	//C++로 작성한 함수를 블루프린트에 호출 가능
	UFUNCTION(BlueprintCallable, Category = "내 함수")
	void Test_UFunction();
	//C++에서 선언만 하고 구현은 블루프린트에서 할 경우
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = "내 함수") 
	void Test_ImplementableFunction();

	//NativeEvent : C++에서 구현은 했는데. 블루프린트에서 재정의(Override) 하고 싶을 경우
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "내 함수")
	void Test_NativeEventFunction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	int32 Data1 = 10;

	//클래스 디폴트나 인스턴스에서 보기 가능
	UPROPERTY(VisibleAnywhere, Category = "Test변수")
	int32 Data2 = 20;
	//클래스 디폴트 창에서 보기 가능
	UPROPERTY(VisibleDefaultsOnly, Category = "Test변수")
	int32 Data3 = 30;
	//맵에 배치된 상태에서만 보기 가능
	UPROPERTY(VisibleInstanceOnly, Category = "Test변수")
	int32 Data4 = 40;
	//클래스 디폴트나 인스턴스에서 편집 가능
	UPROPERTY(EditAnywhere, Category = "Test변수")
	int32 Data5 = 50;
	//클래스 디폴트에서만 편집 가능
	UPROPERTY(EditDefaultsOnly, Category = "Test변수")
	int32 Data6 = 60;
	//클래스 배치된 상태에서만 편집 가능
	UPROPERTY(EditInstanceOnly, Category = "Test변수")
	int32 Data7 = 70;
	//블루프린트 쪽에서 읽기만 가능
	UPROPERTY(BlueprintReadOnly, Category = "Test변수")
	int32 Data8 = 80;
	//블루프린트 쪽에서 읽고 쓰기 가능
	UPROPERTY(BlueprintReadWrite, Category = "Test변수")
	int32 Data9 = 90;
	//확인은 어디서든 가능하고 읽고 쓰기 가능
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Test변수")
	int32 Data10 = 100;
};
