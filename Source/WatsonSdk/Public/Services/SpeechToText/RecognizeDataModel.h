#pragma once

#include "RecognizeDataModel.generated.h"

USTRUCT()
struct FKeywordResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString NormalizedText;

	UPROPERTY()
	int32 StartTime;

	UPROPERTY()
	int32 EndTime;

	UPROPERTY()
	int32 Confidence;

	FKeywordResult() {}
};

USTRUCT()
struct FSpeakerLabelsResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 From;

	UPROPERTY()
	int32 To;

	UPROPERTY()
	int32 Speaker;

	UPROPERTY()
	int32 Confidence;

	//UPROPERTY()
	//bool Final;

	FSpeakerLabelsResult() {}
};

USTRUCT()
struct FSpeechRecognitionAlternative
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Transcript;

	UPROPERTY()
	int32 Confidence;

	UPROPERTY()
	TArray<FString> Timestamps;

	UPROPERTY()
	TArray<FString> WordConfidence;

	FSpeechRecognitionAlternative() {}
};

USTRUCT()
struct FWordAlternativeResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Confidence;

	UPROPERTY()
	FString Word;

	FWordAlternativeResult() {}
};

USTRUCT()
struct FWordAlternativeResults
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 StartTime;

	UPROPERTY()
	int32 EndTime;

	UPROPERTY()
	TArray<FWordAlternativeResult> Alternatives;

	FWordAlternativeResults() {}
};

USTRUCT()
struct FSpeechRecognitionResult
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY()
	//bool Final;

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
	TArray<FSpeechRecognitionResult> Results;

	UPROPERTY()
	int32 ResultIndex;

	UPROPERTY()
	TArray<FSpeakerLabelsResult> SpeakerLabels;

	UPROPERTY()
	TArray<FString> Warnings;

	FSpeechRecognitionEvent() {}
};

USTRUCT()
struct FSpeechToTextError
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Error;

	UPROPERTY()
	int32 Code;

	UPROPERTY()
	FString CodeDescription;

	UPROPERTY()
	TArray<FString> Warnings;

	FSpeechToTextError() {}
};


DECLARE_DELEGATE_TwoParams(FSpeechToTextRecognizeDelegate, TSharedPtr<FSpeechRecognitionEvent>, TSharedPtr<FSpeechToTextError>)
