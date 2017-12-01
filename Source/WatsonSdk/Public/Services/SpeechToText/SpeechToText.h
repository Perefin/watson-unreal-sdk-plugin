#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "SpeechToTextRecognizeModel.h"
#include "SpeechToText.generated.h"

UCLASS()
class WATSONSDK_API USpeechToText : public UWatsonService
{
	GENERATED_BODY()

public:
	USpeechToText();

	FSpeechToTextRecognizeRequest* Recognize(TArray<uint8> AudioData, const FString& AudioModel = "en-US_BroadbandModel", const FString& ContentType = "audio/l16;rate=16000;channels=1;");

private:
	void OnRecognize(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
