#pragma once

#include "TextToSpeechSynthesizeModel.generated.h"

USTRUCT()
struct FTextToSpeechAudio
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<uint8> audioData;
	
	UPROPERTY()
	uint32 audioLength;
	
	FTextToSpeechAudio() {}
};


DECLARE_DELEGATE_OneParam(FTextToSpeechSynthesizeAudioSuccess, TSharedPtr<FTextToSpeechAudio>)
USTRUCT()
struct FTextToSpeechSynthesizeAudioRequest: public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	TSharedPtr<FTextToSpeechAudio> Progress;
	FTextToSpeechSynthesizeAudioSuccess OnSuccess;
	FTextToSpeechSynthesizeAudioRequest() {
		Progress = MakeShareable(new FTextToSpeechAudio());
	}
};
