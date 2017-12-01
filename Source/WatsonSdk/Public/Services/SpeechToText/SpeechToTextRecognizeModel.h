#pragma once

#include "SpeechToTextRecognizeModel.generated.h"

USTRUCT()
struct FSpeechToTextRecognizeKeywordResult
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString normalized_text;
	
	UPROPERTY()
	int32 start_time;
	
	UPROPERTY()
	int32 end_time;
	
	UPROPERTY()
	int32 confidence;
	
	FSpeechToTextRecognizeKeywordResult() {}
};

USTRUCT()
struct FSpeechToTextRecognizeSpeakerLabelsResult
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 from;
	
	UPROPERTY()
	int32 to;
	
	UPROPERTY()
	int32 speaker;
	
	UPROPERTY()
	int32 confidence;

	//UPROPERTY()
	//bool final;

	FSpeechToTextRecognizeSpeakerLabelsResult() {}
};

USTRUCT()
struct FSpeechToTextRecognizeAlternative
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString transcript;
	
	UPROPERTY()
	int32 confidence;
	
	UPROPERTY()
	TArray<FString> timestamps;
	
	UPROPERTY()
	TArray<FString> word_confidence;

	FSpeechToTextRecognizeAlternative() {}
};

USTRUCT()
struct FSpeechToTextRecognizeWordAlternativeResult
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 confidence;
	
	UPROPERTY()
	FString word;

	FSpeechToTextRecognizeWordAlternativeResult() {}
};

USTRUCT()
struct FSpeechToTextRecognizeWordAlternativeResults
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 start_time;
	
	UPROPERTY()
	int32 end_time;
	
	UPROPERTY()
	TArray<FSpeechToTextRecognizeWordAlternativeResult> alternatives;

	FSpeechToTextRecognizeWordAlternativeResults() {}
};

USTRUCT()
struct FSpeechToTextRecognizeResult
{
	GENERATED_USTRUCT_BODY()
	
	//UPROPERTY()
	//bool final;
	
	UPROPERTY()
	TArray<FSpeechToTextRecognizeAlternative> alternatives;
	
	UPROPERTY()
	TMap<FString, FSpeechToTextRecognizeKeywordResult> keyword_results;
	
	UPROPERTY()
	TArray<FSpeechToTextRecognizeWordAlternativeResults> word_alternatives;

	FSpeechToTextRecognizeResult() {}
};

USTRUCT()
struct FSpeechToTextRecognizeResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<FSpeechToTextRecognizeResult> results;
	
	UPROPERTY()
	int32 result_index;
	
	UPROPERTY()
	TArray<FSpeechToTextRecognizeSpeakerLabelsResult> speaker_labels;
	
	UPROPERTY()
	TArray<FString> warnings;

	FSpeechToTextRecognizeResponse() {}
};


DECLARE_DELEGATE_OneParam(FSpeechToTextRecognizeSuccess, TSharedPtr<FSpeechToTextRecognizeResponse>)
USTRUCT()
struct FSpeechToTextRecognizeRequest: public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FSpeechToTextRecognizeSuccess OnSuccess;
	FSpeechToTextRecognizeRequest() {}
};

