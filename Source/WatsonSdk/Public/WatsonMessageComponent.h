// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Authentication.h"
#include "Conversation.h"
#include "TextToSpeech.h"
#include "SpeechToText.h"
#include "Components/ActorComponent.h"
#include "WatsonMessageComponent.generated.h"


UCLASS(BlueprintType, ClassGroup=(WatsonSDK), meta=(BlueprintSpawnableComponent))
class WATSONSDK_API UWatsonMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWatsonMessageComponent();
	UFUNCTION(BlueprintCallable)
	UConversation* CreateConversation(const FAuthentication& Authentication);
	UFUNCTION(BlueprintCallable)
	UTextToSpeech* CreateTextToSpeech(const FAuthentication& Authentication);
	UFUNCTION(BlueprintCallable)
	USpeechToText* CreateSpeechToText(const FAuthentication& Authentication);

private:
	//UPROPERTY(BlueprintReadWrite)
		TArray<UWatsonService*> ServiceList;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
