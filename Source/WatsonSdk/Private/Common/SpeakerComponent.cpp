// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeakerComponent.h"


// Sets default values for this component's properties
USpeakerComponent::USpeakerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AudioPCMComponent = NewObject<USoundWaveProcedural>();
	AudioPCMComponent->SampleRate = 16000;
	AudioPCMComponent->NumChannels = 1;
	AudioPCMComponent->Duration = INDEFINITELY_LOOPING_DURATION;
	AudioPCMComponent->SoundGroup = SOUNDGROUP_Voice;
	AudioPCMComponent->bLooping = false;
	AudioPCMComponent->bProcedural = true;

	AudioOutputComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioOutputComponent"));
	AudioOutputComponent->bAutoActivate = true;
	AudioOutputComponent->bAlwaysPlay = true;
}


// Called when the game starts
void USpeakerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpeakerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpeakerComponent::PlayAudio(TArray<uint8> AudioData, int32 AudioLength)
{
	
	if (AudioPCMComponent != nullptr)
	{
		AudioOutputComponent->SetSound(AudioPCMComponent);
		AudioPCMComponent->QueueAudio(AudioData.GetData(), AudioData.GetAllocatedSize());
		AudioOutputComponent->Play();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound Not Created Trying Again"));
		AudioPCMComponent = NewObject<USoundWaveProcedural>();
		AudioPCMComponent->SampleRate = 16000;
		AudioPCMComponent->NumChannels = 1;
		AudioPCMComponent->Duration = INDEFINITELY_LOOPING_DURATION;
		AudioPCMComponent->SoundGroup = SOUNDGROUP_Voice;
		AudioPCMComponent->bLooping = false;
		AudioPCMComponent->bProcedural = true;
		AudioOutputComponent->SetSound(AudioPCMComponent);
		AudioPCMComponent->QueueAudio(AudioData.GetData(), AudioData.GetAllocatedSize());
		AudioOutputComponent->Play();
	}
}

void USpeakerComponent::PlayAudioResponse(FTextToSpeechAudio Response)
{
	PlayAudio(Response.audioData, Response.audioLength);
}