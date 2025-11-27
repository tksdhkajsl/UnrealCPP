// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NiagaraTestActor.h"
#include "NiagaraComponent.h"

// Sets default values
ANiagaraTestActor::ANiagaraTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Root);
	Effect->SetRelativeLocation(FVector(0, 0, 100));

}

// Called when the game starts or when spawned
void ANiagaraTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANiagaraTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANiagaraTestActor::TestColorChange(const FLinearColor& InColor)
{
	Effect->SetColorParameter(TEXT("EffectColor"), InColor);
	//Effect->SetNiagaraVariableLinearColor(TEXT("EffectColor"), InColor);
}

