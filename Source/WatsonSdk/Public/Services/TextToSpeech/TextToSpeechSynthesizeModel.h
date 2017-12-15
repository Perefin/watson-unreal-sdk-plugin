#pragma once

#include "TextToSpeechSynthesizeModel.generated.h"

USTRUCT(BlueprintType)
struct FTextToSpeechAudio
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> audioData;
	
	UPROPERTY(BlueprintReadWrite)
	int32 audioLength;
	
	FTextToSpeechAudio() {}
};

USTRUCT(BlueprintType)
struct FTextToSpeechAudioInt32
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		TArray<uint8> audioData;

	UPROPERTY(BlueprintReadWrite)
		int32 audioLength;

	FTextToSpeechAudioInt32() {}

	FTextToSpeechAudioInt32(FTextToSpeechAudio original) 
	{
		audioData = original.audioData;
		audioLength = original.audioLength;
	}

};

DECLARE_DYNAMIC_DELEGATE_OneParam(FTextToSpeechSynthesizeAudioSuccess, FTextToSpeechAudio, TextToSpeechAudio);
USTRUCT(BlueprintType)
struct FTextToSpeechSynthesizeAudioRequest: public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()

	FTextToSpeechAudio Progress;
public:
	UPROPERTY(BlueprintReadWrite)
	FTextToSpeechSynthesizeAudioSuccess OnSuccess;
	FTextToSpeechSynthesizeAudioRequest() {
		Progress = FTextToSpeechAudio();
	}
};
