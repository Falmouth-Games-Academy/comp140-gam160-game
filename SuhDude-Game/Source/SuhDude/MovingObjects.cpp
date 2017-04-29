// Fill out your copyright notice in the Description page of Project Settings.

#include "SuhDude.h"
#include "MovingObjects.h"


// Sets default values
AMovingObjects::AMovingObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingObjects::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingObjects::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	AddActorWorldOffset(SpeedOfMovement);

}

