#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "SpeechToTextRecognizeModel.h"
#include "SpeechToText.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWatsonRequestFailure, FString, Error);

UCLASS(BlueprintType, Category = "Watson")
class WATSONSDK_API USpeechToText : public UWatsonService
{
	GENERATED_BODY()

public:
	USpeechToText();

	UFUNCTION(BlueprintCallable)
		FSpeechToTextRecognizeRequest Recognize(TArray<uint8> AudioData, const FString& AudioModel = "en-US_BroadbandModel", const FString& ContentType = "audio/l16;rate=16000;channels=1;");
	
	UFUNCTION(BlueprintCallable)
	void MakeSpeechToTextRequest(UObject* object, TArray<uint8> AudioData, FSpeechToTextRecognizeSuccess OnSuccess, FWatsonRequestFailure OnFailure);
private:
	void OnRecognize(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
