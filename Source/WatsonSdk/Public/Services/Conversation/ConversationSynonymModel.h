#pragma once

#include "ConversationModel.h"
#include "ConversationSynonymModel.generated.h"

USTRUCT()
struct FConversationSynonym
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString synonym;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    FConversationSynonym() {}
};

USTRUCT()
struct FConversationSynonymList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationSynonym> synonyms;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationSynonymList() {}
};


DECLARE_DELEGATE_OneParam(FConversationListSynonymsSuccess, TSharedPtr<FConversationSynonymList>)
USTRUCT()
struct FConversationListSynonymsRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListSynonymsSuccess OnSuccess;
	FConversationListSynonymsRequest() {}
};


DECLARE_DELEGATE_OneParam(FConversationGetSynonymSuccess, TSharedPtr<FConversationSynonym>)
USTRUCT()
struct FConversationGetSynonymRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetSynonymSuccess OnSuccess;
	FConversationGetSynonymRequest() {}
};