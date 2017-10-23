#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "RecognizeDataModel.h"
#include "SpeechToText.generated.h"

UCLASS()
class WATSONSDK_API USpeechToText : public UWatsonService
{
	GENERATED_BODY()

public:
	USpeechToText();



//////////////////////////////////////////////////////////////////////////
// Sessionless Recognize Audio

private:
	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FSpeechToTextRecognizePendingRequest>> PendingRecognizeRequests;
	void OnRecognizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	/**
	* Sends audio and returns transcription results for a sessionless recognition request.
	*
	* @param AudioData		The raw audio to be transcribed
	* @param AudioModel	The identifier of the model to be used for the recognition request
	* @return				Delegate called when the request is complete.
	*/
	FSpeechToTextRecognizePendingRequest* Recognize(TArray<uint8> AudioData, const FString& AudioModel = FString("en-US_BroadbandModel"));
};
