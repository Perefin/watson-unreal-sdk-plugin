// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeakerComponent.h"


// Sets default values for this component's properties
USpeakerComponent::USpeakerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AudioPCMComponent = CreateDefaultSubobject<USoundWaveProcedural>(TEXT("AudioPCMComponent"));
	//AudioPCMComponent = NewObject<USoundWaveProcedural>(this, USoundWaveProcedural::StaticClass());
	AudioPCMComponent->SampleRate = 16000;
	AudioPCMComponent->NumChannels = 1;
	AudioPCMComponent->Duration = INDEFINITELY_LOOPING_DURATION;
	AudioPCMComponent->SoundGroup = SOUNDGROUP_Voice;
	AudioPCMComponent->bLooping = false;
	AudioPCMComponent->bProcedural = true;

	AudioOutputComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioOutputComponent"));
	AudioOutputComponent->bAutoActivate = true;
	AudioOutputComponent->bAlwaysPlay = true;
	/*AudioOutputComponent->SetSound(AudioPCMComponent);*/
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

void USpeakerComponent::PlayAudio(const TArray<uint8>& AudioData, uint32 AudioLength)
{
	AudioOutputComponent->SetSound(AudioPCMComponent);
	AudioPCMComponent->QueueAudio(AudioData.GetData(), AudioLength / 16 * 16);
	AudioOutputComponent->Play();
}

void USpeakerComponent::PlayAudio(const TArray<uint8>& AudioData, int32 AudioLength)
{
	PlayAudio(AudioData,(uint32) AudioLength);
}

