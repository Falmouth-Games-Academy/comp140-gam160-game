// Fill out your copyright notice in the Description page of Project Settings.

#include "SuhDude.h"
#include "Server.h"


// Sets default values
AServer::AServer(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void AServer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AServer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
FString AServer::PrimeForAdress(FString MessageToConvert)
{
	FString stringToCheck = MessageToConvert;
	FString PrimedString = stringToCheck.Replace(TEXT(" "), TEXT("%20")); //Replacing the string so that spaces become "%20" so that the post message can support spaces.
	return PrimedString;
}
/*Makes the http get request to send the score and the message. It is called and used inside the "httpActor" blueprint class*/
void AServer::UploadToServer(FString Score, FString Message)
{
	FString FirstGetVariable = "&Message="; //setting the X to parse it into the the httprequest string
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AServer::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL("http://ec2-50-112-74-134.us-west-2.compute.amazonaws.com/link.py?Score="+Score + FirstGetVariable + Message);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}
void AServer::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		int32 recievedInt = JsonObject->GetIntegerField("Score");

		//Output it to the engine
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString("hello world"));
	}
}