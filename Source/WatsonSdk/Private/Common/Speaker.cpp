#pragma once

#include "Speaker.h"

USpeaker::USpeaker()
{
	AudioPCMComponent = CreateDefaultSubobject<USoundWaveProcedural>(TEXT("SpeakerAudioPCMComponent"));
	AudioPCMComponent->SampleRate = 16000;
	AudioPCMComponent->NumChannels = 1;
	AudioPCMComponent->Duration = INDEFINITELY_LOOPING_DURATION;
	AudioPCMComponent->SoundGroup = SOUNDGROUP_Voice;
	AudioPCMComponent->bLooping = false;
	AudioPCMComponent->bProcedural = true;

	AudioOutputComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpeakerAudioOutputComponent"));
	AudioOutputComponent->bAutoActivate = true;
	AudioOutputComponent->bAlwaysPlay = true;
	AudioOutputComponent->SetSound(AudioPCMComponent);
}

void USpeaker::PlayAudio(const TArray<uint8>& AudioData, uint32 AudioLength)
{
	AudioPCMComponent->QueueAudio(AudioData.GetData(), AudioLength / 16 * 16);
	AudioOutputComponent->Play();
}