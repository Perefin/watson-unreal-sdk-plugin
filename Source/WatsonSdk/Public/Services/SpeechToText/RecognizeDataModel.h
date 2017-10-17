#pragma once

#include "RecognizeDataModel.generated.h"

USTRUCT()
struct FKeywordResult
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
	
	FKeywordResult() {}
};

USTRUCT()
struct FSpeakerLabelsResult
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

	FSpeakerLabelsResult() {}
};

USTRUCT()
struct FSpeechRecognitionAlternative
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

	FSpeechRecognitionAlternative() {}
};

USTRUCT()
struct FWordAlternativeResult
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 confidence;
	
	UPROPERTY()
	FString word;

	FWordAlternativeResult() {}
};

USTRUCT()
struct FWordAlternativeResults
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 start_time;
	
	UPROPERTY()
	int32 end_time;
	
	UPROPERTY()
	TArray<FWordAlternativeResult> alternatives;

	FWordAlternativeResults() {}
};

USTRUCT()
struct FSpeechRecognitionResult
{
	GENERATED_USTRUCT_BODY()
	
	//UPROPERTY()
	//bool final;
	
	UPROPERTY()
	TArray<FSpeechRecognitionAlternative> alternatives;
	
	UPROPERTY()
	TMap<FString, FKeywordResult> keyword_results;
	
	UPROPERTY()
	TArray<FWordAlternativeResults> word_alternatives;

	FSpeechRecognitionResult() {}
};

USTRUCT()
struct FSpeechRecognitionEvent
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<FSpeechRecognitionResult> results;
	
	UPROPERTY()
	int32 result_index;
	
	UPROPERTY()
	TArray<FSpeakerLabelsResult> speaker_labels;
	
	UPROPERTY()
	TArray<FString> warnings;

	FSpeechRecognitionEvent() {}
};

USTRUCT()
struct FSpeechToTextError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString error;
	
	UPROPERTY()
	int32 code;
	
	UPROPERTY()
	FString code_description;
	
	UPROPERTY()
	TArray<FString> warnings;
	
	FSpeechToTextError() {}
};


DECLARE_DELEGATE_TwoParams(FSpeechToTextRecognizeDelegate, TSharedPtr<FSpeechRecognitionEvent>, TSharedPtr<FSpeechToTextError>)
