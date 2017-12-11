#pragma once

#include "ConversationModel.h"
#include "ConversationValueModel.generated.h"

USTRUCT()
struct FConversationValue
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString value;

    TSharedPtr<FJsonObject> metadata;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    UPROPERTY()
    TArray<FString> synonyms;

    UPROPERTY()
    TArray<FString> patterns;

    UPROPERTY()
    TArray<FString> type;

    FConversationValue() {}
};

USTRUCT()
struct FConversationValueList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationValue> values;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationValueList() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationListValuesSuccess, FConversationValueList, ConversationValueList);
USTRUCT()
struct FConversationListValuesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListValuesSuccess OnSuccess;
	FConversationListValuesRequest() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationGetValueSuccess, FConversationValue, ConversationValue);
USTRUCT()
struct FConversationGetValueRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetValueSuccess OnSuccess;
	FConversationGetValueRequest() {}
};