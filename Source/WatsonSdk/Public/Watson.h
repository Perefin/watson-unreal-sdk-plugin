#pragma once

#include "CoreMinimal.h"
#include "Common/Authentication.h"
#include "Conversation.h"
#include "TextToSpeech.h"
#include "SpeechToText.h"
#include "Watson.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Watson")
class WATSONSDK_API UWatson : public UObject
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
	UConversation* CreateConversation(const FAuthentication& Authentication);
	//UFUNCTION(BlueprintCallable)
	UTextToSpeech* CreateTextToSpeech(const FAuthentication& Authentication);
	//UFUNCTION(BlueprintCallable)
	USpeechToText* CreateSpeechToText(const FAuthentication& Authentication);

private:
	UPROPERTY()
	TArray<UWatsonService*> ServiceList;
};