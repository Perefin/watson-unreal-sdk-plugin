#pragma once

#include "ConversationModel.h"
#include "ConversationExampleModel.h"
#include "ConversationIntentModel.generated.h"

USTRUCT()
struct FConversationIntent
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString intent;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    UPROPERTY()
    FString description;

    UPROPERTY()
    TArray<FConversationExample> examples;

    FConversationIntent() {}
};

USTRUCT()
struct FConversationIntentList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationIntent> intents;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationIntentList() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationListIntentsSuccess, FConversationIntentList, ConversationIntentList);
USTRUCT(BlueprintType)
struct FConversationListIntentsRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FConversationListIntentsSuccess OnSuccess;
	FConversationListIntentsRequest() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationGetIntentSuccess, FConversationIntent, ConversationIntent);
USTRUCT(BlueprintType)
struct FConversationGetIntentRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FConversationGetIntentSuccess OnSuccess;
	FConversationGetIntentRequest() {}
};