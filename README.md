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
// Create UConversation
UConversation* MyConversation = CreateDefaultSubobject<UConversation>(TEXT("Conversation"));
MyConversation->Authorization.username = /* username */;
MyConversation->Authorization.password = /* password */;

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
USpeechToText* MySpeechToText = CreateDefaultSubobject<USpeechToText>(TEXT("SpeechToText"));
MySpeechToText->Authorization.username = /* username */;
MySpeechToText->Authorization.password = /* password */;

// Create and send request
FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
Request->OnSuccess.BindUObject(this, &AMyActor::OnSpeechToTextRecognize);
Request->OnFailure.BindUObject(this, &AMyActor::OnSpeechToTextFailure);
Request->Send();
```
### Watson Text To Speech
```cpp
// Create UTextToSpeech
UTextToSpeech* MyTextToSpeech = CreateDefaultSubobject<UTextToSpeech>(TEXT("TextToSpeech"));
MyTextToSpeech->Authorization.username = /* username */;
MyTextToSpeech->Authorization.password = /* password */;

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
	// Speaker
	UPROPERTY() USpeaker* MySpeaker;
	
	// Microphone
	UPROPERTY() UMicrophone* MyMicrophone;
	void OnMicrophoneStart();
	void OnMicrophoneStop();

	// Conversation
	UPROPERTY() UConversation* MyConversation;
	void OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response);
	void OnConversationFailure(FString Error);

	// Speech To Text
	UPROPERTY() USpeechToText* MySpeechToText;
	void OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response);
	void OnSpeechToTextFailure(FString Error);
	
	// Text To Speech
	UPROPERTY() UTextToSpeech* MyTextToSpeech;
	void OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response);
	void OnTextToSpeechFailure(FString Error);
};

```

### MyActor.cpp
``` cpp
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
	MyConversation->Authorization.Username = "f1eeed67-c6ad-4a58-a5bd-e0c05f542949";
	MyConversation->Authorization.Password = "xHAVHLnYjeUz";

	// Create Watson Speech To Text
	MySpeechToText = CreateDefaultSubobject<USpeechToText>(TEXT("SpeechToText"));
	MySpeechToText->Authorization.Username = "970ef92d-ce9e-4734-8f5e-e258f9c53275";
	MySpeechToText->Authorization.Password = "xsfnJXmVVNQ2";

	// Create Watson Text To Speech
	MyTextToSpeech = CreateDefaultSubobject<UTextToSpeech>(TEXT("TextToSpeech"));
	MyTextToSpeech->Authorization.Username = "f6cba252-5c58-43e8-a40c-8d1cd2d184ba";
	MyTextToSpeech->Authorization.Password = "lGEkbFTiaDDw";
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

	// Make Conversation Request
	FConversationMessageRequest ConversationRequest;
	ConversationRequest.input.text = "Hello there, how are you?";
	FConversationMessagePendingRequest* ConvRequest = MyConversation->Message("1a2cdfcd-e42a-43f7-947c-ba44ebf4f2af", ConversationRequest);
	ConvRequest->OnSuccess.BindUObject(this, &AMyActor::OnConversationMessage);
	ConvRequest->OnFailure.BindUObject(this, &AMyActor::OnConversationFailure);
	ConvRequest->Send();

	// Make Text To Speech Request
	FTextToSpeechSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = "Hello there, how are you?";
	FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
	T2sRequest->OnSuccess.BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);
	T2sRequest->OnFailure.BindUObject(this, &AMyActor::OnTextToSpeechFailure);
	T2sRequest->Send();
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
	UE_LOG(LogTemp, Warning, TEXT("Conversation Success: %s"), *Response->output.text[0]);
}

void AMyActor::OnConversationFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Error: %s"), *Error);
}

void AMyActor::OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response)
{
	for (auto& result : Response->results) {
		for (auto& alternative : result.alternatives) {
			UE_LOG(LogTemp, Warning, TEXT("Speech To Text Success: %s"), *alternative.transcript);
		}
	}
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
