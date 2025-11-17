// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickup.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/TimelineComponent.h"
#include "Player/InventoryOwner.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//root collision add (box)
	//무기 외형: skeletalmesh
	//오버랩 영역: 스피어컬리전
	//이펙트 그대로
	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->InitSphereRadius(1.0f);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BaseRoot);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->AddRelativeRotation(FRotator(0, 0, -10.0f));

	PickupOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap"));
	PickupOverlap->SetupAttachment(BaseRoot);
	PickupOverlap->InitSphereRadius(100.0f);
	PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeLine"));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupOverlap)
	{
		PickupOverlap->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPickupBeginOverlap);
	}

	if (PickupTimeline)
	{
		if (ScaleCurve)
		{
			FOnTimelineFloat ScaleUpdateDelegate;
			ScaleUpdateDelegate.BindUFunction(this, FName("OnScaleUpdate"));
			PickupTimeline->AddInterpFloat(ScaleCurve, ScaleUpdateDelegate);

			FOnTimelineEvent ScaleFinishDelegate;
			ScaleFinishDelegate.BindUFunction(this, FName("OnScaleFinish"));
			PickupTimeline->SetTimelineFinishedFunc(ScaleFinishDelegate);
		}

		PickupTimeline->SetPlayRate(1 / Duration);
	}

	bPickuped = false;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddWorldRotation(FRotator(0, RotateSpeed * DeltaTime, 0));
}

void APickup::OnPickup_Implementation(AActor* Target)
{
	
	// 자신을 먹은 대상에게 자기가 가지고 있는 무기를 알려줘야 함
	if (!bPickuped)
	{
		UE_LOG(LogTemp, Log, TEXT("OnPickup_Implementation 실행"));
		bPickuped = true;
		PickupOwner = Target;

		//물리 시뮬레이션 끄기 (필수! 안 끄면 위치 이동이 튐)
		BaseRoot->SetSimulatePhysics(false);
		//충돌체 때문에 캐릭터가 밀리지 않도록 콜리전 무시 설정
		BaseRoot->SetCollisionProfileName(TEXT("NoCollision"));
		
		PickupStartLocation = GetActorLocation();
		PickupTimeline->PlayFromStart();// 타임라인 시작
		
	}
}

void APickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	//연출을 넣어도되는 곳
	//UE_LOG(LogTemp, Log, TEXT("Pickup Overlap"));
}

void APickup::OnScaleUpdate(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("OnScaleUpdate"));
	FVector NewScale = FVector::One() * Value;
	SetActorScale3D(NewScale);

	// 2. 위치 이동 로직 (추가)
	if (PickupOwner.IsValid())
	{
		// 타겟 위치 (캐릭터의 위치 + 약간 위쪽으로 오프셋을 주면 배꼽/가슴 쪽으로 날아옴)
		FVector TargetLocation = PickupOwner->GetActorLocation() + FVector(0, 0, 50.0f);

		// Value가 1.0 -> 0.0 으로 변한다면, 진행률(Alpha)은 반대로 0.0 -> 1.0 이어야 함
		// 만약 커브가 0->1이라면 그냥 Value를 쓰면 됌.
		// (ScaleCurve가 1->0으로 작아지는 커브라고 가정했을 때)
		float Alpha = 1.0f - Value;

		// 시작 위치에서 타겟 위치로 Alpha만큼 이동
		FVector NewLocation = FMath::Lerp(PickupStartLocation, TargetLocation, Alpha);

		SetActorLocation(NewLocation);
	}
}

void APickup::OnScaleFinish()
{

	// 자신을 먹은 대상에게 자기가 가지고 있는 무기를 알려줘야 함
	if (PickupOwner.IsValid() && PickupOwner->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupOwner.Get(), PickupItem);
	}
}