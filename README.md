# watson-sdk-plugin

## Setup
1. Clone the plugin to your project's <code>Plugins/</code> directory
2. Add the following module dependencies to your project's <code>.Build.cs</code>
```csharp
  PrivateDependencyModuleNames.AddRange(new string[] {
    "WatsonSDK",
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
### Watson Conversation
```cpp
UConversation* MyConversation = CreateDefaultSubobject<UConversation>(TEXT("Conversation"));
MyConversation->Authorization.username = /* username */;
MyConversation->Authorization.password = /* password */;

FConversationMessageRequest ConversationRequest;
ConversationRequest.input.text = "Hello there, how are you?";
MyConversation->Message(/* workspace */, ConversationRequest)->BindUObject(this, &AMyActor::OnConversationMessage);

void AMyActor::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response, TSharedPtr<FConversationMessageError> Error)
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->output.text[0]);
}
```
### Watson Speech To Text
```cpp
USpeechToText* MySpeechToText = CreateDefaultSubobject<USpeechToText>(TEXT("SpeechToText"));
MySpeechToText->Authorization.username = /* username */;
MySpeechToText->Authorization.password = /* password */;

MySpeechToText->Recognize(MySpeaker->GetRecording(), "en-US_BroadbandModel")->BindUObject(this, &AMyActor::OnSpeechToTextRecognize);

void AMyActor::OnSpeechToTextRecognize(TSharedPtr<FSpeechRecognitionEvent> Response, TSharedPtr<FSpeechToTextError> Error)
{
  for (auto& result : Response->results)
    for (auto& alternative : result.alternatives)
      UE_LOG(LogTemp, Warning, TEXT("%s"), *alternative.transcript);
}
```
### Watson Text To Speech
```cpp
UTextToSpeech* MyTextToSpeech = CreateDefaultSubobject<UTextToSpeech>(TEXT("TextToSpeech"));
MyTextToSpeech->Authorization.username = /* username */;
MyTextToSpeech->Authorization.password = /* password */;

FSynthesizeRequest SynthesisRequest;
SynthesisRequest.text = "Hello there, how are you?";
MyTextToSpeech->Synthesize(SynthesisRequest)->BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);

void AMyActor::OnTextToSpeechSynthesize(TSharedPtr<FSynthesizeResponse> Response, TSharedPtr<FSynthesizeError> Error)
{
  MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}
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
#include "WatsonSdk/Public/Services/Conversation/Conversation.h"
#include "WatsonSdk/Public/Services/SpeechToText/SpeechToText.h"
#include "WatsonSdk/Public/Services/TextToSpeech/TextToSpeech.h"
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
	// Microphone & Speaker
	UPROPERTY() USpeaker* MySpeaker;
	UPROPERTY() UMicrophone* MyMicrophone;
	void OnMicrophoneStart();
	void OnMicrophoneStop();

	// Conversation
	UPROPERTY() UConversation* MyConversation;
	void OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response, TSharedPtr<FConversationMessageError> Error);

	// Speech To Text
	UPROPERTY() USpeechToText* MySpeechToText;
	void OnSpeechToTextRecognize(TSharedPtr<FSpeechRecognitionEvent> Response, TSharedPtr<FSpeechToTextError> Error);
	
	// Text To Speech
	UPROPERTY() UTextToSpeech* MyTextToSpeech;
	void OnTextToSpeechSynthesize(TSharedPtr<FSynthesizeResponse> Response, TSharedPtr<FSynthesizeError> Error);
};
```

### MyActor.cpp
``` cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create Speaker & Microphone
	MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));
	MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

	// Create Watson Conversation
	MyConversation = CreateDefaultSubobject<UConversation>(TEXT("Conversation"));
	MyConversation->Authorization.Username = /* Username */;
	MyConversation->Authorization.Password = /* Password */;

	// Create Watson Speech To Text
	MySpeechToText = CreateDefaultSubobject<USpeechToText>(TEXT("SpeechToText"));
	MySpeechToText->Authorization.Username = /* Username */;
	MySpeechToText->Authorization.Password = /* Password */;

	// Create Watson Text To Speech
	MyTextToSpeech = CreateDefaultSubobject<UTextToSpeech>(TEXT("TextToSpeech"));
	MyTextToSpeech->Authorization.Username = /* Username */;
	MyTextToSpeech->Authorization.Password = /* Password */;
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// Make Conversation Request
	FConversationMessageRequest ConversationRequest;
	ConversationRequest.input.text = "Hello there, how are you?";
	MyConversation->Message("1a2cdfcd-e42a-43f7-947c-ba44ebf4f2af", ConversationRequest)->BindUObject(this, &AMyActor::OnConversationMessage);

	// Make Speech To Text Request
	FSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = "Hello there, how are you?";
	MyTextToSpeech->Synthesize(SynthesisRequest)->BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyActor::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Microphone", IE_Pressed, this, &AMyActor::OnMicrophoneStart);
	InputComponent->BindAction("Microphone", IE_Released, this, &AMyActor::OnMicrophoneStop);
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
	
	// Make Text To Speech Request
	MySpeechToText->Recognize(MyMicrophone->GetRecording(), "en-US_BroadbandModel")->BindUObject(this, &AMyActor::OnSpeechToTextRecognize);
}

///////////////////////////////////////////////////////////////////////
// Delegate Bindings

void AMyActor::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response, TSharedPtr<FConversationMessageError> Error)
{
	UE_LOG(LogTemp, Warning, TEXT("SUCCESS %s"), *Response->output.text[0]);
}

void AMyActor::OnSpeechToTextRecognize(TSharedPtr<FSpeechRecognitionEvent> Response, TSharedPtr<FSpeechToTextError> Error)
{
	for (auto& result : Response->results) {
		for (auto& alternative : result.alternatives) {
			UE_LOG(LogTemp, Warning, TEXT("SUCCESS %s"), *alternative.transcript);
		}
	}
}

void AMyActor::OnTextToSpeechSynthesize(TSharedPtr<FSynthesizeResponse> Response, TSharedPtr<FSynthesizeError> Error)
{
	MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}
```
