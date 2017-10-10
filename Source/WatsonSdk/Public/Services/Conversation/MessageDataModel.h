#pragma once

#include "MessageDataModel.generated.h"

USTRUCT()
struct FConversationContext
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString conversation_id;
	FConversationContext() {}
};

USTRUCT()
struct FConversationRuntimeIntent
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString intent;
	UPROPERTY() float confidence;
	FConversationRuntimeIntent() {}
};

USTRUCT()
struct FConversationRuntimeEntity
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString entity;
	UPROPERTY() TArray<int32> location;
	UPROPERTY() FString value;
	UPROPERTY() float confidence;
	FConversationRuntimeEntity() {}
};

USTRUCT()
struct FConversationLogMessage
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString level;
	UPROPERTY() FString msg;
	FConversationLogMessage() {}
};

USTRUCT()
struct FConversationOutputData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() TArray<FConversationLogMessage> log_messages;
	UPROPERTY() TArray<FString> text;
	UPROPERTY() TArray<FString> nodes_visited;
	FConversationOutputData() {}
};

USTRUCT()
struct FConversationInputData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString text;
	FConversationInputData() {}
};

USTRUCT()
struct FConversationMessageRequest
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FConversationInputData input;
	UPROPERTY() bool alternate_intents;
	UPROPERTY() FConversationContext context;
	UPROPERTY() TArray<FConversationRuntimeEntity> entities;
	UPROPERTY() TArray<FConversationRuntimeIntent> intents;
	UPROPERTY() FConversationOutputData output;
	FConversationMessageRequest() {}
};

USTRUCT()
struct FConversationMessageInput
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString text;
	FConversationMessageInput() {}
};

USTRUCT()
struct FConversationMessageResponse
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FConversationMessageInput input;
	UPROPERTY() TArray<FConversationRuntimeIntent> intents;
	UPROPERTY() TArray<FConversationRuntimeEntity> entities;
	UPROPERTY() bool alternate_intents;
	UPROPERTY() FConversationContext context;
	UPROPERTY() FConversationOutputData output;
	FConversationMessageResponse() {}
};

USTRUCT()
struct FConversationMessageErrorDetail
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString message;
	UPROPERTY() FString path;
	FConversationMessageErrorDetail() {}
};

USTRUCT()
struct FConversationMessageError
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString error;
	UPROPERTY() TArray<FConversationMessageErrorDetail> errors;
	FConversationMessageError() {}
};