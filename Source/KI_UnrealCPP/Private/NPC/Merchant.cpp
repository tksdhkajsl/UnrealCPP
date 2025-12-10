// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Merchant.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/Interactor.h"
#include "Player/ActionPlayerController.h"

// Sets default values
AMerchant::AMerchant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidget"));
	TextWidgetComponent->SetupAttachment(RootComponent);
	TextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TextWidgetComponent->SetVisibility(false);

	InteractionBound = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionBound"));
	InteractionBound->SetupAttachment(RootComponent);
	InteractionBound->SetSphereRadius(300.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	LookCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LookCamera"));
	LookCamera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AMerchant::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AMerchant::OnInteractionBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMerchant::OnInteractionEndOverlap);	
}

void AMerchant::OnInteraction_Implementation()
{
	// 상점 열기
	UWorld* world = GetWorld();
	if (AActionPlayerController* pc = world->GetFirstPlayerController<AActionPlayerController>())
	{
		//UE_LOG(LogTemp, Log, TEXT("상점을 엽니다. (%s)"), *GetActorLabel());
		pc->OpenShopWidget(this);
	}
}

void AMerchant::OnInteractionBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->Implements<UInteractor>())
	{
		TextWidgetComponent->SetVisibility(true);
		IInteractor::Execute_AddInteractionTarget(OtherActor, this);
	}
}

void AMerchant::OnInteractionEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->Implements<UInteractor>())
	{
		IInteractor::Execute_ClearInteractionTarget(OtherActor, this);
		TextWidgetComponent->SetVisibility(false);
	}
}
