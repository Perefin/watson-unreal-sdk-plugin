#pragma once

#include "ConversationModel.h"
#include "ConversationExampleModel.generated.h"

USTRUCT()
struct FConversationExample
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString text;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    FConversationExample() {}
};

USTRUCT()
struct FConversationExampleList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationExample> examples;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationExampleList() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationListExamplesSuccess, FConversationExampleList, ConversationExampleList);
USTRUCT()
struct FConversationListExamplesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListExamplesSuccess OnSuccess;
	FConversationListExamplesRequest() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationGetExampleSuccess, FConversationExample, ConversationExample);
USTRUCT()
struct FConversationGetExampleRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetExampleSuccess OnSuccess;
	FConversationGetExampleRequest() {}
};