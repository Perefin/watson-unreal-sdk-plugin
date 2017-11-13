# Watson SDK Plugin

## Before you Begin
Ensure that you have the following prerequisites:

* An IBM Cloud account. If you don't have one, [sign up](https://console.bluemix.net/registration/).
* Unreal Engine 4.18. 

## Setup
1. If it doesn't exist already, create <code>Plugins/</code> folder in your project's directory
2. Clone the plugin to your project's <code>Plugins/</code> directory
3. In Unreal Engine 4, Create Pawn C++ Class
4. Add Sample Pawn Code to MyPawn.h 
	* Replace <code>YOURPROJECTNAME</code> in <code>class <YOURPROJECTNAME_API></code> with your project name. (ex: <code>class WATSONUNREALENGINEPROJECT_API</code>)
5. Add Sample Pawn Code to MyPawn.cpp
	* Insert your IBM Cloud credentials in the following format: <code>"username","password"</code>
	_Note: See Configure your service credentials below_
6. Add the following module dependencies to your project's <code>.Build.cs</code> (ex: <code>YOURPROJECTNAME.Build.cs</code>)
```csharp
  PrivateDependencyModuleNames.AddRange(new string[] {
    "WatsonSdk",
    "Voice"
  });
```
7. Add the following to your engine config <code>DefaultEngine.ini</code>
```
  [OnlineSubsystem]
  bHasVoiceEnabled=true

  [Voice]
  bEnabled=true
```
8. Compile
9. If you want a trigger key, Create Microphone Action Mapping
10. Place Pawn in environment

## Configure your service credentials
To create instances of Watson services and their credentials, follow the steps below.

**Note:** Service credentials are different from your IBM Cloud account username and password.

1. Determine which services to configure.
1. If you have configured the services already, complete the following steps. Otherwise, go to step 3.
    1. Log in to Bluemix at https://bluemix.net.
    1. Click the service you would like to use.
    1. Click **Service credentials**.
    1. Click **View credentials** to access your credentials.
1. If you need to configure the services that you want to use, complete the following steps.
    1. Log in to Bluemix at https://bluemix.net.
    1. Click the **Create service** button.
    1. Under **Watson**, select which service you would like to create an instance of and click that service.
    1. Give the service and credential a name. Select a plan and click the **Create** button on the bottom.
    4. Click **Service Credentials**.
    5. Click **View credentials** to access your credentials.
1. Paste your credentials in the matching line item in the MyPawn.cpp file
1. Your service credentials can be used to instantiate Watson Services within your application. Most services also support tokens which you can instantiate the service with as well.

The credentials for each service contain either a `username`, `password` and endpoint `url` **or** an `apikey` and endpoint `url`.

**WARNING:** You are responsible for securing your own credentials. Any user with your service credentials can access your service instances!

## Create Microphone Action Mapping
1. Edit - > Project Settings -> Input -> Action Mapping -> Choose a name -> Choose a microphone hotkey -> Close


## Usage
### Watson
```cpp
// Create Watson Instance
MyWatson = CreateDefaultSubobject<UWatson>(TEXT("Watson"));
```
### Watson Conversation
```cpp
// Create UConversation
UConversation* MyConversation = MyWatson->CreateConversation(FAuthentication(/* username */, /* password */));

// Create and send request
FConversationMessageRequest ConversationRequest;
ConversationRequest.input.text = "Hello there, how are you?";

FConversationMessagePendingRequest* Request = MyConversation->Message("1a2cdfcd-e42a-43f7-947c-ba44ebf4f2af", ConversationRequest);
Request->OnSuccess.BindUObject(this, &AMyPawn::OnConversationMessage);
Request->OnFailure.BindUObject(this, &AMyPawn::OnConversationFailure);
Request->Send();
```
### Watson Speech To Text
```cpp
// Create USpeechToText
USpeechToText* MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication(/* username */, /* password */));

// Create and send request
FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
Request->OnSuccess.BindUObject(this, &AMyPawn::OnSpeechToTextRecognize);
Request->OnFailure.BindUObject(this, &AMyPawn::OnSpeechToTextFailure);
Request->Send();
```
### Watson Text To Speech
```cpp
// Create UTextToSpeech
UTextToSpeech* MyTextToSpeech = MyWatson->CreateTextToSpeech(FAuthentication(/* username */, /* password */));

// Create and send request
FTextToSpeechSynthesizeRequest SynthesisRequest;
SynthesisRequest.text = "Hello there, how are you?";

FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
T2sRequest->OnSuccess.BindUObject(this, &AMyPawn::OnTextToSpeechSynthesize);
T2sRequest->OnFailure.BindUObject(this, &AMyPawn::OnTextToSpeechFailure);
T2sRequest->Send();
```
### Speaker
```cpp
USpeaker* MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));

MySpeaker->PlayAudio(/* raw audio */, /* raw audio length */);
```
### Microphone
```cpp
UMicrophone* MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

MyMicrophone->StartRecording();
MyMicrophone->StopRecording();
MyMicrophone->GetRecording();
```

## Sample Pawn Code

### MyPawn.h
```cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Watson.h"
#include "Common/Speaker.h"
#include "Common/Microphone.h"
#include "MyPawn.generated.h"

//Enter Your Project Name Below
UCLASS()
class <YOURPROJECTNAME_API> AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);

protected:
	// Speaker
	UPROPERTY() USpeaker* MySpeaker;

	// Microphone
	UPROPERTY() UMicrophone* MyMicrophone;
	void OnMicrophoneStart();
	void OnMicrophoneStop();

	// Watson
	UPROPERTY() UWatson* MyWatson;

	// Conversation
	UPROPERTY() UConversation* MyConversation;
	TSharedPtr<FConversationMessageResponse> LastResponse;
	void OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response);
	void OnConversationFailure(FString Error);

	// TextToSpeech
	UPROPERTY() UTextToSpeech* MyTextToSpeech;
	void OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response);
	void OnTextToSpeechFailure(FString Error);

	// SpeechToText
	UPROPERTY() USpeechToText* MySpeechToText;
	void OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response);
	void OnSpeechToTextFailure(FString Error);
};
```

### MyPawn.cpp
``` cpp
// Here's a snippet that allows you to converse with Watson Conversation using Speech-To-Text & Text-To-Speech.
// Make sure to create a Microphone Action Mapping for push-to-talk behavior.

#include "MyPawn.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyWatson = CreateDefaultSubobject<UWatson>(TEXT("Watson"));
	MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));
	MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

	MyConversation = MyWatson->CreateConversation(FAuthentication("ENTER WATSON SERVICE CREDENTIALS HERE"));
	MyTextToSpeech = MyWatson->CreateTextToSpeech(FAuthentication("ENTER WATSON SERVICE CREDENTIALS HERE"));
	MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication("970ef92d-xxxxxxxxxxxxxxx", "xxxxxxXXXXXX"));
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Microphone", IE_Pressed, this, &AMyPawn::OnMicrophoneStart);
	InputComponent->BindAction("Microphone", IE_Released, this, &AMyPawn::OnMicrophoneStop);
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPawn::OnMicrophoneStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Starting..."));
	MyMicrophone->StartRecording();
}

void AMyPawn::OnMicrophoneStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Stopping..."));
	MyMicrophone->StopRecording();

	// Make Speech To Text Request
	FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
	Request->OnSuccess.BindUObject(this, &AMyPawn::OnSpeechToTextRecognize);
	Request->OnFailure.BindUObject(this, &AMyPawn::OnSpeechToTextFailure);
	Request->Send();
}

void AMyPawn::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Success: %s"), *Response->output.text.Last());

	LastResponse = Response;

	// Make Text To Speech Request
	FTextToSpeechSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = Response->output.text.Last();

	FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
	T2sRequest->OnSuccess.BindUObject(this, &AMyPawn::OnTextToSpeechSynthesize);
	T2sRequest->OnFailure.BindUObject(this, &AMyPawn::OnTextToSpeechFailure);
	T2sRequest->Send();
}

void AMyPawn::OnConversationFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Error: %s"), *Error);
}

void AMyPawn::OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Speech To Text Success: %s"), *Response->results[0].alternatives[0].transcript);

	// Make Conversation Request
	FConversationMessageRequest ConversationRequest;
	ConversationRequest.input.text = Response->results[0].alternatives[0].transcript;
	if (LastResponse.Get() != nullptr)
	{
		ConversationRequest.context = LastResponse->context;
		ConversationRequest.output = LastResponse->output;
	}

	FConversationMessagePendingRequest* ConvRequest = MyConversation->Message("1a2cdfcd-e42a-43f7-947c-ba44ebf4f2af", ConversationRequest);
	ConvRequest->OnSuccess.BindUObject(this, &AMyPawn::OnConversationMessage);
	ConvRequest->OnFailure.BindUObject(this, &AMyPawn::OnConversationFailure);
	ConvRequest->Send();
}

void AMyPawn::OnSpeechToTextFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Speech To Text Error: %s"), *Error);
}

void AMyPawn::OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response)
{
	MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}

void AMyPawn::OnTextToSpeechFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Text To Speech Error: %s"), *Error);
}
```
