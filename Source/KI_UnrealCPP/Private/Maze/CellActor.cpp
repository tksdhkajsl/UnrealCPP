// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/CellActor.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACellActor::ACellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionProfileName(TEXT("BlockAll"));

	UStaticMeshComponent* gate;
	gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateNorth"));
	gate->SetupAttachment(BaseMesh);
	gate->SetRelativeLocation(FVector::ForwardVector * (CellHalfSize - GateHalfThickness));
	GateMeshArray.Add(gate);
	
	gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateEast"));
	gate->SetupAttachment(BaseMesh);
	gate->SetRelativeLocation(FVector::RightVector * (CellHalfSize - GateHalfThickness));
	gate->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	GateMeshArray.Add(gate);

	gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateSouth"));
	gate->SetupAttachment(BaseMesh);
	gate->SetRelativeLocation(FVector::BackwardVector * (CellHalfSize - GateHalfThickness));
	gate->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	GateMeshArray.Add(gate);

	gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateWest"));
	gate->SetupAttachment(BaseMesh);
	gate->SetRelativeLocation(FVector::LeftVector * (CellHalfSize - GateHalfThickness));
	gate->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GateMeshArray.Add(gate);

	UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(BaseMesh);
	arrow->SetRelativeLocation(FVector(0, 0, 200));

	this->SetCanBeDamaged(false);

}

// Called when the game starts or when spawned
void ACellActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACellActor::InitializeCell(FCellData* InCellData)
{
	if (InCellData)
	{
		Path = InCellData->Path;		
		OpenGate();
	}
}

void ACellActor::TestSetPath(EDirectionType Direction)
{
	Path = Direction;
	OpenGate();
}

void ACellActor::OpenGate()
{
	for (int i = 0; i < 4; i++)
	{
		EDirectionType dir = static_cast<EDirectionType>(1 << i);
		if (IsPath(dir))	// 벽이 아니면
		{
			GateMeshArray[i]->SetVisibility(false);
			GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

}

bool ACellActor::IsPath(EDirectionType Direction)
{
	return (Path & Direction) != EDirectionType::None;
}
