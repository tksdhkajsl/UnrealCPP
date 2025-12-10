// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeActor.generated.h"

UCLASS()
class KI_UNREALCPP_API AMazeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnPreMapGenerate();
	void OnPostMapGenerate();

private:	
	class FMazeData* MakeMaze();
	void SpawnCells(FMazeData* Maze);
	void ClearMaze(class FMazeData*& Maze);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	int32 Width = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	int32 Height = 3;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze")
	int32 RandomSeed = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze")
	TSubclassOf<class ACellActor> CellActorClass = nullptr;


};
