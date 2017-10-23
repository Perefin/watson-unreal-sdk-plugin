#pragma once

#include "Watson.h"

UConversation* UWatson::CreateConversation(const FAuthentication& Authentication)
{
	UConversation* Conversation = NewObject<UConversation>(this, UConversation::StaticClass());
	Conversation->SetAuthentication(Authentication);
	ServiceList.Add(Conversation);
	return Conversation;
}

USpeechToText* UWatson::CreateSpeechToText(const FAuthentication& Authentication)
{
	USpeechToText* SpeechToText = NewObject<USpeechToText>(this, USpeechToText::StaticClass());
	SpeechToText->SetAuthentication(Authentication);
	ServiceList.Add(SpeechToText);
	return SpeechToText;
}

UTextToSpeech* UWatson::CreateTextToSpeech(const FAuthentication& Authentication)
{
	UTextToSpeech* TextToSpeech = NewObject<UTextToSpeech>(this, UTextToSpeech::StaticClass());
	TextToSpeech->SetAuthentication(Authentication);
	ServiceList.Add(TextToSpeech);
	return TextToSpeech;
}