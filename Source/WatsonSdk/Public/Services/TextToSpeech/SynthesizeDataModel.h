#pragma once

#include "SynthesizeDataModel.generated.h"

USTRUCT()
struct FSynthesizeRequest
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString text;
	
	FSynthesizeRequest() {}
};

USTRUCT()
struct FSynthesizeResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<uint8> audioData;
	
	UPROPERTY()
	uint32 audioLength;
	
	FSynthesizeResponse() {}
};

USTRUCT()
struct FSynthesizeError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString error;
	
	UPROPERTY()
	int32 code;
	
	FSynthesizeError() {}
};

DECLARE_DELEGATE_TwoParams(FTextToSpeechSynthesizeDelegate, TSharedPtr<FSynthesizeResponse>, TSharedPtr<FSynthesizeError>)
struct FSynthesisProgress
{
	TSharedPtr<FSynthesizeResponse> Response;
	TSharedPtr<FTextToSpeechSynthesizeDelegate> Delegate;
	FSynthesisProgress() {}
	FSynthesisProgress(TSharedPtr<FTextToSpeechSynthesizeDelegate> Delegate) : Response(MakeShareable(new FSynthesizeResponse)), Delegate(Delegate) {}
};