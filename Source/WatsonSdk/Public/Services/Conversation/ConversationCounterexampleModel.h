#pragma once

#include "ConversationModel.h"
#include "ConversationCounterexampleModel.generated.h"

USTRUCT()
struct FConversationCounterexample
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString text;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    FConversationCounterexample() {}
};

USTRUCT()
struct FConversationCounterexampleList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationCounterexample> counterexamples;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationCounterexampleList() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationListCounterexamplesSuccess, FConversationCounterexampleList, ConversationCounterexampleList);
USTRUCT()
struct FConversationListCounterexamplesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListCounterexamplesSuccess OnSuccess;
	FConversationListCounterexamplesRequest() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationGetCounterexampleSuccess, FConversationCounterexample, ConversationCounterexample);
USTRUCT()
struct FConversationGetCounterexampleRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetCounterexampleSuccess OnSuccess;
	FConversationGetCounterexampleRequest() {}
};