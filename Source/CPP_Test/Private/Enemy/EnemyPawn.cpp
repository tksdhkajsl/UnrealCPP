// Fill out your copyright notice in the Description page of Project Settings.



#include "Enemy/EnemyPawn.h"
#include "Enemy/DamagePopupActor.h"
#include "Framework/DamagePopupSubsystem.h"
#include "Framework/EnemyTrackingSubsystem.h"
#include "Player/ResourceComponent.h"
#include "Data/DropItemDataTableRow.h"
#include "Item/Pickup.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	PopupLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PopupLocation"));
	PopupLocation->SetupAttachment(Mesh);
	PopupLocation->SetRelativeLocation(FVector(0, 0, 100));

	Resource = CreateDefaultSubobject<UResourceComponent>(TEXT("Resource"));

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AEnemyPawn::OnTakeDamage);

	if (UWorld* world = GetWorld())
	{
		if (UEnemyTrackingSubsystem* enemyTracker = world->GetSubsystem<UEnemyTrackingSubsystem>())
		{
			enemyTracker->RegistEnemy();
		}
	}
}

void AEnemyPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* world = GetWorld())
	{
		if (UEnemyTrackingSubsystem* enemyTracker = world->GetSubsystem<UEnemyTrackingSubsystem>())
		{
			enemyTracker->UnregistEnemy();
		}
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyPawn::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage()
	if (Resource->IsAlive())
	{
		if (!bInvincible || !FMath::IsNearlyEqual(LastDamage, Damage))
		{
			UE_LOG(LogTemp, Log, TEXT("Damage : %.1f"), Damage);

			Resource->AddHealth(-Damage);
			//ADamagePopupActor* actor = GetWorld()->SpawnActor<ADamagePopupActor>(
			//	DamagePopupClass, PopupLocation->GetComponentToWorld());
			//if (actor)
			//{
			//	actor->PopupActivate(Damage);
			//}

			UDamagePopupSubsystem* popupSystem = GetWorld()->GetSubsystem<UDamagePopupSubsystem>();
			popupSystem->ShowDamagePopup(Damage, PopupLocation->GetComponentLocation());

			if (Resource->IsAlive())
			{
				// 한번에 연속해서 데미지 입는것 방지
				bInvincible = true;
				LastDamage = Damage;

				FTimerDelegate resetInvincibleDelegate = FTimerDelegate::CreateWeakLambda(
					this,
					[this]()
					{
						bInvincible = false;
					});	// this가 파괴되면 람다는 더 이상 실행되지 않는다.

				GetWorldTimerManager().ClearTimer(InvincibleTimer);
				GetWorldTimerManager().SetTimer(
					InvincibleTimer,
					resetInvincibleDelegate,
					0.1f, false);
			}
			else
			{
				OnDie();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("이 적은 이미 죽어있다."));
	}
	
}

void AEnemyPawn::DropItems()
{
	//for (const auto& item : DropItemInfo)
	//{
	//	item.DropRate;
	//	item.DropItemClass;
	//}

	if (DropItemTable)
	{
		//TArray<FDropItemData_TableRow*> AllRows;
		//DropItemTable->GetAllRows<FDropItemData_TableRow>(TEXT("Rows"), AllRows);

		APickup* pickup = nullptr;
		TMap<FName, uint8*> RowMap = DropItemTable->GetRowMap();

		// 중복으로 당첨 가능
		for (const auto& element : RowMap)
		{
			pickup = nullptr;
			FDropItemDataTableRow* row = (FDropItemDataTableRow*)element.Value;
			if (FMath::FRand() <= row->DropRate)
			{
				pickup = GetWorld()->SpawnActor<APickup>(
					row->DropItemClass,
					GetActorLocation() + FVector::UpVector * 200.0f,
					GetActorRotation());
			}
			if (pickup)
			{
				UE_LOG(LogTemp, Log, TEXT("Drop Success : %s"), *pickup->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Drop empty"));
			}
		}

		//// 전체 가중치 사용하는 방식(1개 보장)
		//float totalWeight = 0.0f;
		//for (const auto& element : RowMap)
		//{
		//	FDropItemData_TableRow* row = (FDropItemData_TableRow*)element.Value;
		//	totalWeight += row->DropRate;
		//}
		//float randomSelect = FMath::FRandRange(0, totalWeight);
		//float currentWeight = 0.0f;
		//for (const auto& element : RowMap)
		//{
		//	FDropItemData_TableRow* row = (FDropItemData_TableRow*)element.Value;
		//	currentWeight += row->DropRate;
		//	if (randomSelect < currentWeight)
		//	{
		//		pickup = GetWorld()->SpawnActor<APickup>(
		//			row->DropItemClass,
		//			GetActorLocation() + FVector::UpVector * 200.0f,
		//			GetActorRotation());
		//		break;
		//	}
		//}	

		//if (pickup)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("Drop Success : %s"), *pickup->GetName());
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Log, TEXT("Drop empty"));
		//}
	}
}

void AEnemyPawn::OnDie()
{
	DropItems();
	Destroy();	// 죽었으면 삭제
}

