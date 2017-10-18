#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "Common/Authorization.h"
#include "Common/Configuration.h"
#include "SynthesizeDataModel.h"
#include "TextToSpeech.generated.h"

UCLASS()
class WATSONSDK_API UTextToSpeech : public UObject
{
	GENERATED_BODY()

public:
	FAuthorization Authorization;
	FConfiguration Configuration;
	UTextToSpeech();

	//////////////////////////////////////////////////////////////////////////
	// Synthesize Audio

private:
	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FSynthesisProgress>> PendingSynthesisRequests;
	void OnSynthesizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSynthesizeProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

public:
	/**
	* Synthesizes text to spoken audio, returning the synthesized audio stream as an array of bytes. 
	*
	* @param Request		The text to be synthesized
	* @return				Delegate called when the request is complete with the audio stream
	*/
	TSharedPtr<FTextToSpeechSynthesizeDelegate> Synthesize(const FSynthesizeRequest& Request, const FString& Voice = FString("en-US_MichaelVoice"));

};
