# watson-sdk-plugin

## Setup
1. Clone the plugin to your project's <code>Plugins/</code> directory
2. Add the following module dependencies to your project's <code>.Build.cs</code>
```csharp
  PrivateDependencyModuleNames.AddRange(new string[] {
    "WatsonSdk",
    "Voice"
  });
```
3. Add the following to your engine config <code>DefaultEngine.ini</code>
```
  [OnlineSubsystem]
  DefaultPlatformService=Null
  bHasVoiceEnabled=true
  
  [Voice]
  bEnabled=true
```

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
Request->OnSuccess.BindUObject(this, &AMyActor::OnConversationMessage);
Request->OnFailure.BindUObject(this, &AMyActor::OnConversationFailure);
Request->Send();
```
### Watson Speech To Text
```cpp
// Create USpeechToText
USpeechToText* MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication(/* username */, /* password */));

// Create and send request
FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
Request->OnSuccess.BindUObject(this, &AMyActor::OnSpeechToTextRecognize);
Request->OnFailure.BindUObject(this, &AMyActor::OnSpeechToTextFailure);
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
T2sRequest->OnSuccess.BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);
T2sRequest->OnFailure.BindUObject(this, &AMyActor::OnTextToSpeechFailure);
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

## Sample Actor

### MyActor.h
```cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Watson.h"
#include "Common/Speaker.h"
#include "Common/Microphone.h"
#include "MyActor.generated.h"

UCLASS()
class WATSONSDKTESTBED_API AMyActor : public APawn
{
	GENERATED_BODY()

public:	 
	AMyActor();
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

### MyActor.cpp
``` cpp
// Here's a snippet that allows you to converse with Watson Conversation using Speech-To-Text & Text-To-Speech.
// Make sure to create a Microphone Action Mapping for push-to-talk behavior.

#include "MyActor.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyWatson = CreateDefaultSubobject<UWatson>(TEXT("Watson"));
	MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));
	MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

	MyConversation = MyWatson->CreateConversation(FAuthentication("f1eeed67-c6ad-4a58-a5bd-e0c05f542949", "xHAVHLnYjeUz"));
	MyTextToSpeech = MyWatson->CreateTextToSpeech(FAuthentication("f6cba252-5c58-43e8-a40c-8d1cd2d184ba", "lGEkbFTiaDDw"));
	MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication("970ef92d-ce9e-4734-8f5e-e258f9c53275", "xsfnJXmVVNQ2"));
}

void AMyActor::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Microphone", IE_Pressed, this, &AMyActor::OnMicrophoneStart);
	InputComponent->BindAction("Microphone", IE_Released, this, &AMyActor::OnMicrophoneStop);
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyActor::OnMicrophoneStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Starting..."));
	MyMicrophone->StartRecording();
}

void AMyActor::OnMicrophoneStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Stopping..."));
	MyMicrophone->StopRecording();
	
	// Make Speech To Text Request
	FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
	Request->OnSuccess.BindUObject(this, &AMyActor::OnSpeechToTextRecognize);
	Request->OnFailure.BindUObject(this, &AMyActor::OnSpeechToTextFailure);
	Request->Send();
}

void AMyActor::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Success: %s"), *Response->output.text.Last());
	
	LastResponse = Response;

	// Make Text To Speech Request
	FTextToSpeechSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = Response->output.text.Last();

	FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
	T2sRequest->OnSuccess.BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);
	T2sRequest->OnFailure.BindUObject(this, &AMyActor::OnTextToSpeechFailure);
	T2sRequest->Send();
}

void AMyActor::OnConversationFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Error: %s"), *Error);
}

void AMyActor::OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response)
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
	ConvRequest->OnSuccess.BindUObject(this, &AMyActor::OnConversationMessage);
	ConvRequest->OnFailure.BindUObject(this, &AMyActor::OnConversationFailure);
	ConvRequest->Send();
}

void AMyActor::OnSpeechToTextFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Speech To Text Error: %s"), *Error);
}

void AMyActor::OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response)
{
	MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}

void AMyActor::OnTextToSpeechFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Text To Speech Error: %s"), *Error);
}
```
