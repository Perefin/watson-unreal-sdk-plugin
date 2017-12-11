#pragma once

#include "ConversationMessageModel.generated.h"

USTRUCT(BlueprintType)
struct FConversationMessageRuntimeIntent
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString intent;
	
	UPROPERTY(BlueprintReadWrite)
	float confidence;
	
	FConversationMessageRuntimeIntent() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageRuntimeEntity
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString entity;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> location;
	
	UPROPERTY(BlueprintReadWrite)
	FString value;
	
	UPROPERTY(BlueprintReadWrite)
	float confidence;
	
	FConversationMessageRuntimeEntity() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageLogMessage
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString level;
	
	UPROPERTY(BlueprintReadWrite)
	FString msg;
	
	FConversationMessageLogMessage() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageOutputData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FConversationMessageLogMessage> log_messages;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> text;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> nodes_visited;
	
	FConversationMessageOutputData() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageInputData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString text;
	
	FConversationMessageInputData() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageRequest
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FConversationMessageInputData input;
	
	UPROPERTY(BlueprintReadWrite)
	bool alternate_intents = false;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FConversationMessageRuntimeEntity> entities;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FConversationMessageRuntimeIntent> intents;
	
	UPROPERTY(BlueprintReadWrite)
	FConversationMessageOutputData output;

	TSharedPtr<FJsonObject> context;

	FConversationMessageRequest() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageInput
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString text;
	
	FConversationMessageInput() {}
};

USTRUCT(BlueprintType)
struct FConversationMessageResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FConversationMessageInput input;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FConversationMessageRuntimeIntent> intents;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FConversationMessageRuntimeEntity> entities;
	
	UPROPERTY(BlueprintReadWrite)
	bool alternate_intents;
	
	UPROPERTY(BlueprintReadWrite)
	FConversationMessageOutputData output;

	TSharedPtr<FJsonObject> context;
	
	FConversationMessageResponse() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationMessageSuccess, FConversationMessageResponse, ConversationMessageResponse);
USTRUCT(BlueprintType)
struct FConversationMessagePendingRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FConversationMessageSuccess OnSuccess;
	FConversationMessagePendingRequest() {}
};