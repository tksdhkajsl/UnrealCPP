// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/MazeActor.h"
#include "Maze/MazeData.h"

// Sets default values
AMazeActor::AMazeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMazeActor::BeginPlay()
{
	Super::BeginPlay();

	MazeData = new FMazeData();
	MazeData->MakeMaze(4, 4, 10);
}

void AMazeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MazeData->ClearMaze();
	delete MazeData;
	MazeData = nullptr;
	Super::EndPlay(EndPlayReason);
}



