#pragma once

#include "CoreMinimal.h"
#include "Common/Authentication.h"
#include "Conversation.h"
#include "TextToSpeech.h"
#include "SpeechToText.h"
#include "Watson.generated.h"

UCLASS()
class WATSONSDK_API UWatson : public UObject
{
	GENERATED_BODY()

public:
	UConversation* CreateConversation(const FAuthentication& Authentication);
	UTextToSpeech* CreateTextToSpeech(const FAuthentication& Authentication);
	USpeechToText* CreateSpeechToText(const FAuthentication& Authentication);

private:
	UPROPERTY()
	TArray<UWatsonService*> ServiceList;
};