// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PickupFactorySubsystem.h"
#include "Settings/PickupSettings.h"
#include "Data/DataTableRows.h"

void UPickupFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("UPickupFactorySubsystem : 초기화 시작"))

	LoadPickupClassesMap();
}

APickup* UPickupFactorySubsystem::SpawnPickup(EItemCode InCode, FVector InLocation, FRotator InRotator)
{
	APickup* spawnedPickup = nullptr;
	if (PickupClasses.Contains(InCode))
	{
		UWorld* world = GetWorld();
		spawnedPickup = world->SpawnActor<APickup>(PickupClasses[InCode], InLocation, InRotator);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem : 설정되지 않은 Pickup을 생성하려고 합니다."));
	}
	return spawnedPickup;
}

void UPickupFactorySubsystem::LoadPickupClassesMap()
{
	const UPickupSettings* settings = GetDefault<UPickupSettings>();
	if (settings && !settings->PickupDataTable.IsNull())
	{
		UDataTable* dataTable = settings->PickupDataTable.LoadSynchronous();
		if (dataTable)
		{
			TArray<FPickData_TableRow*> allRows;
			dataTable->GetAllRows(TEXT("PickupDataTable"), allRows);
			for (const FPickData_TableRow* row : allRows)
			{
				if (PickupClasses.Contains(row->PickupCode))
				{
					UE_LOG(LogTemp, Warning, TEXT("UPickupFactorySubsystem : 키 값이 중복입니다. 이전 값은 덮어써집니다."));
				}
				PickupClasses.Add(row->PickupCode, row->PickupClass);
			}			
			UE_LOG(LogTemp, Log, TEXT("UPickupFactorySubsystem : DataTable 로딩 완료."))
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem : DataTable 로딩 실패!!!"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem : PickupDataTable is null!!!"))
	}
}
