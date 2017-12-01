// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voice.h"
#include "Interfaces/VoiceCapture.h"
#include "Components/ActorComponent.h"
#include "MicrophoneComponent.generated.h"


UCLASS( ClassGroup=(WatsonSDK), meta=(BlueprintSpawnableComponent))
class WATSONSDK_API UMicrophoneComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMicrophoneComponent();

	UFUNCTION(BlueprintCallable)
	void StartRecording();
	UFUNCTION(BlueprintCallable)
	void StopRecording();
	TArray<uint8> GetRecording();

	void BeginDestroy() override;

	UPROPERTY(BlueprintReadWrite)
		TArray<uint8> VoiceCaptureBuffer;

	UPROPERTY(BlueprintReadWrite)
		int32 VoiceCaptureSize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
private:
	TSharedPtr<class IVoiceCapture> VoiceCapture;
};
