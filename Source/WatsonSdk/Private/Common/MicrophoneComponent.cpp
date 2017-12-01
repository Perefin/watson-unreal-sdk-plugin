// Fill out your copyright notice in the Description page of Project Settings.

#include "MicrophoneComponent.h"


// Sets default values for this component's properties
UMicrophoneComponent::UMicrophoneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture();
	uint32 AvailableVoiceData;
	if (VoiceCapture->GetCaptureState(AvailableVoiceData) == EVoiceCaptureState::UnInitialized)
	{
		VoiceCapture->Init("", 16000, 1);
	}
	VoiceCaptureBuffer.SetNumUninitialized(10000);
	VoiceCaptureSize = 0;
}


// Called when the game starts
void UMicrophoneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMicrophoneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	uint32 AvailableVoiceDataInBytes = 0;
	if (VoiceCapture.IsValid() && VoiceCapture->GetCaptureState(AvailableVoiceDataInBytes) == EVoiceCaptureState::Type::Ok && AvailableVoiceDataInBytes > 0)
	{
		TArray<uint8> TempCaptureBuffer;
		TempCaptureBuffer.SetNumUninitialized(AvailableVoiceDataInBytes);

		uint32 ReadVoiceDataBytes = 0;
		VoiceCapture->GetVoiceData(TempCaptureBuffer.GetData(), AvailableVoiceDataInBytes, ReadVoiceDataBytes);

		VoiceCaptureBuffer.Append(TempCaptureBuffer.GetData(), ReadVoiceDataBytes);
		VoiceCaptureSize += ReadVoiceDataBytes;
	}
}

void UMicrophoneComponent::StartRecording()
{
	VoiceCaptureBuffer.Empty();
	VoiceCaptureSize = 0;
	VoiceCapture->Start();
}

void UMicrophoneComponent::StopRecording()
{
	VoiceCapture->Stop();
}

TArray<uint8> UMicrophoneComponent::GetRecording()
{
	VoiceCaptureBuffer.Shrink();
	return VoiceCaptureBuffer;
}

void UMicrophoneComponent::BeginDestroy()
{
	Super::BeginDestroy();
	VoiceCapture->Shutdown();
	VoiceCaptureBuffer.Empty();
	VoiceCaptureSize = 0;
}


