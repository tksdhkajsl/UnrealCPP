// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DamagePopupActor.h"
#include "Widget/DamageWidget.h"
#include "Components/WidgetComponent.h"
#include "Framework/DamagePopupSubsystem.h"

// Sets default values
ADamagePopupActor::ADamagePopupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	SetRootComponent(DamageWidgetComponent);

	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);	// 항상 카메라에 보이도록 스크린 스페이스 기준으로 설정
	DamageWidgetComponent->SetDrawAtDesiredSize(true);
}

void ADamagePopupActor::PopupActivate(float Damage)
{
	//UE_LOG(LogTemp, Log, TEXT("PopupActivate"));
	if (DamageWidget)
	{
		//UE_LOG(LogTemp, Log, TEXT("PopupActivate : DamageWidget vaild."));
		DamageWidget->SetDamageText(Damage);
		DamageWidget->PlayPopupAnimation();
	}

	GetWorldTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&ADamagePopupActor::PopupDeactivate,
		LifeTime,
		false
	);
}

void ADamagePopupActor::PopupDeactivate()
{
	//UE_LOG(LogTemp, Log, TEXT("PopupDeactivate"));
	if (UWorld* world = GetWorld())
	{
		if (UDamagePopupSubsystem* PoolSystem = world->GetSubsystem<UDamagePopupSubsystem>())
		{
			PoolSystem->ReturnToPool(this);
		}
		else
		{
			Destroy();	// 안전 장치 : 만약을 대비한 것
		}
	}
	else
	{
		Destroy();	// 안전 장치 : 만약을 대비한 것
	}
}

void ADamagePopupActor::BeginPlay()
{
	Super::BeginPlay();
	if (DamageWidgetComponent)
	{
		DamageWidget = Cast<UDamageWidget>(DamageWidgetComponent->GetUserWidgetObject());
	}
}
