// Fill out your copyright notice in the Description page of Project Settings.

#include "SuhDude.h"
#include "CppSpawner.h"

// Sets default values
ACppSpawner::ACppSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACppSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

int ACppSpawner::WhatToSpawn(int score, int difficulty)
{	
	int Weighting;
	// 0 to spawn a friendly. 1 to spawn enemy.
	int Decision;
	Weighting = score * difficulty;
	int RandomNumber = rand() % Weighting;
	if (RandomNumber > score)
	{
		Decision = 1;
	}
	else
	{
		Decision = 0;
	}
	
	return Decision;
}



