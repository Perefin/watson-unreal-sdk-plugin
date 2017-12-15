#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "TextToSpeechSynthesizeModel.h"
#include "TextToSpeech.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Watson")
class WATSONSDK_API UTextToSpeech : public UWatsonService
{
	GENERATED_BODY()

public:
	UTextToSpeech();

	FTextToSpeechSynthesizeAudioRequest* SynthesizeAudio(const FString& Text, const FString& Voice = "en-US_MichaelVoice", const FString& CustomizationId = "", const FString& Accept = "audio/l16;rate=16000;channels=1;");
	UFUNCTION(BlueprintCallable)
	void MakeTextToSpeechRequest(const FString& Text, FTextToSpeechSynthesizeAudioSuccess OnSuccess, FWatsonRequestFailure OnFailure, const FString& Voice = "en-US_MichaelVoice", const FString& CustomizationId = "", const FString& Accept = "audio/l16;rate=16000;channels=1;");
	UFUNCTION(BlueprintCallable)
	FTextToSpeechAudioInt32 GetAudio(FTextToSpeechAudio original);
	

private:
	void OnSynthesizeAudio(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSynthesizeAudioProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
};
