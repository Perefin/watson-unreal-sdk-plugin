// Fill out your copyright notice in the Description page of Project Settings.

#include "WatsonMessageComponent.h"


// Sets default values for this component's properties
UWatsonMessageComponent::UWatsonMessageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWatsonMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWatsonMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UConversation* UWatsonMessageComponent::CreateConversation(const FAuthentication& Authentication)
{
	UConversation* Conversation = NewObject<UConversation>(this, UConversation::StaticClass());
	//UConversation* Conversation = CreateDefaultSubobject<UConversation>(TEXT("Conversation"));
	Conversation->SetAuthentication(Authentication);
	ServiceList.Add(Conversation);
	return Conversation;
}

USpeechToText* UWatsonMessageComponent::CreateSpeechToText(const FAuthentication& Authentication)
{
	USpeechToText* SpeechToText = NewObject<USpeechToText>(this, USpeechToText::StaticClass());
	SpeechToText->SetAuthentication(Authentication);
	ServiceList.Add(SpeechToText);
	return SpeechToText;
}

UTextToSpeech* UWatsonMessageComponent::CreateTextToSpeech(const FAuthentication& Authentication)
{
	UTextToSpeech* TextToSpeech = NewObject<UTextToSpeech>(this, UTextToSpeech::StaticClass());
	TextToSpeech->SetAuthentication(Authentication);
	ServiceList.Add(TextToSpeech);
	return TextToSpeech;
}

FConversationMessageResponse UWatsonMessageComponent::GetLastResponse()
{
	if (!LastResponse.IsValid())
	{
		LastResponse = MakeShareable(new FConversationMessageResponse);
	}
	return *LastResponse;
}

void UWatsonMessageComponent::SendMessage(FString Message, UConversation* MyConversation)
{
	// Create and send request
	FConversationMessageRequest ConversationRequest;
	ConversationRequest.input.text = Message;

	FConversationMessagePendingRequest* Request = MyConversation->Message("1a2cdfcd-e42a-43f7-947c-ba44ebf4f2af", ConversationRequest);
	Request->OnSuccess.BindDynamic(this, &UWatsonMessageComponent::OnConversationMessage);
	Request->OnFailure.BindDynamic(this, &UWatsonMessageComponent::OnConversationFailure);
	Request->Send();
}

void UWatsonMessageComponent::OnConversationMessage(FConversationMessageResponse Response)
{
	if (Response.output.text.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Conversation Success: [No Output Text]"));
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("Conversation Success: %s"), *Response.output.text.Last());

		if (!LastResponse.IsValid())
		{
			LastResponse = MakeShareable(new FConversationMessageResponse);
			*LastResponse = Response;
		}
		else
		{
			*LastResponse = Response;
		}
		// Make Text To Speech Request

		/*FTextToSpeechSynthesizeAudioRequest T2sRequest = MyTextToSpeech->SynthesizeAudio(Response.output.text.Last());
		T2sRequest.OnSuccess.BindDynamic(this, &AMyPawn::OnTextToSpeechSynthesize);
		T2sRequest.OnFailure.BindDynamic(this, &AMyPawn::OnTextToSpeechFailure);
		T2sRequest.Send();*/
	}
}

void UWatsonMessageComponent::OnConversationFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Error: %s"), *Error);
}

void UWatsonMessageComponent::OnTextToSpeechSynthesize(FTextToSpeechAudio Response)
{
	//MySpeaker->PlayAudio(Response.audioData, Response.audioLength);
}

void UWatsonMessageComponent::OnTextToSpeechFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Text To Speech Error: %s"), *Error);
}