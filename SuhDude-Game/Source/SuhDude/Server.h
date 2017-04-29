// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "Http.h"
#include "Server.generated.h"


UCLASS()
class SUHDUDE_API AServer : public AActor
{
	GENERATED_BODY()
	
public:	
	FHttpModule* Http;

	UFUNCTION(BlueprintCallable, category = "Http Request")
		void UploadToServer(FString Score, FString Message);
	UFUNCTION(BlueprintCallable, category = "Http Request")
		FString PrimeForAdress(FString MessageToConvert);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Sets default values for this actor's properties
	AServer(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

};
