#pragma once

#include "SynthesizeDataModel.generated.h"

USTRUCT()
struct FTextToSpeechSynthesizeRequest
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString text;
	
	FTextToSpeechSynthesizeRequest() {}
};

USTRUCT()
struct FTextToSpeechSynthesizeResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<uint8> audioData;
	
	UPROPERTY()
	uint32 audioLength;
	
	FTextToSpeechSynthesizeResponse() {}
};

USTRUCT()
struct FTextToSpeechSynthesizeError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString error;
	
	UPROPERTY()
	int32 code;
	
	FTextToSpeechSynthesizeError() {}
};


DECLARE_DELEGATE_OneParam(FTextToSpeechSynthesizeSuccessDelegate, TSharedPtr<FTextToSpeechSynthesizeResponse>)
DECLARE_DELEGATE_OneParam(FTextToSpeechSynthesizeFailureDelegate, FString)
struct FTextToSpeechSynthesizePendingRequest
{
	TSharedPtr<IHttpRequest> HttpRequest;
	TSharedPtr<FTextToSpeechSynthesizeResponse> Response;
	FTextToSpeechSynthesizeSuccessDelegate OnSuccess;
	FTextToSpeechSynthesizeFailureDelegate OnFailure;
	void Send()
	{
		HttpRequest->ProcessRequest();
	}
};