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

