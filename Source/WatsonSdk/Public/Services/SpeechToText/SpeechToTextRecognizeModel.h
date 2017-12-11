#pragma once

#include "SpeechToTextRecognizeModel.generated.h"

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeKeywordResult
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString normalized_text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 start_time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 end_time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 confidence;

	FSpeechToTextRecognizeKeywordResult() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeSpeakerLabelsResult
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 from;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 to;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 speaker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 confidence;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//bool final;

	FSpeechToTextRecognizeSpeakerLabelsResult() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeAlternative
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString transcript;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 confidence;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FString> timestamps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FString> word_confidence;

	FSpeechToTextRecognizeAlternative() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeWordAlternativeResult
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 confidence;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString word;

	FSpeechToTextRecognizeWordAlternativeResult() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeWordAlternativeResults
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 start_time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 end_time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpeechToTextRecognizeWordAlternativeResult> alternatives;

	FSpeechToTextRecognizeWordAlternativeResults() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeResult
{
	GENERATED_USTRUCT_BODY()

		//UPROPERTY(BlueprintReadWrite, EditAnywhere)
		//bool final;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpeechToTextRecognizeAlternative> alternatives;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, FSpeechToTextRecognizeKeywordResult> keyword_results;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpeechToTextRecognizeWordAlternativeResults> word_alternatives;

	FSpeechToTextRecognizeResult() {}
};

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeResponse
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpeechToTextRecognizeResult> results;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 result_index;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpeechToTextRecognizeSpeakerLabelsResult> speaker_labels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FString> warnings;

	FSpeechToTextRecognizeResponse() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FSpeechToTextRecognizeSuccess, FSpeechToTextRecognizeResponse, SpeechToTextRecognizeResponse);

USTRUCT(BlueprintType)
struct FSpeechToTextRecognizeRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FSpeechToTextRecognizeSuccess OnSuccess;
	FSpeechToTextRecognizeRequest() {}
};

