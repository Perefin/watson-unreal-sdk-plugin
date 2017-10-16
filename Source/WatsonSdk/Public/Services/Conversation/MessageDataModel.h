#pragma once

#include "MessageDataModel.generated.h"

USTRUCT()
struct FConversationContext
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString ConversationId;
	
	FConversationContext() {}
};

USTRUCT()
struct FConversationRuntimeIntent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Intent;
	
	UPROPERTY()
	float Confidence;
	
	FConversationRuntimeIntent() {}
};

USTRUCT()
struct FConversationRuntimeEntity
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString Entity;
	
	UPROPERTY()
	TArray<int32> Location;
	
	UPROPERTY()
	FString Value;
	
	UPROPERTY()
	float Confidence;
	
	FConversationRuntimeEntity() {}
};

USTRUCT()
struct FConversationLogMessage
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString Level;
	
	UPROPERTY()
	FString Msg;
	
	FConversationLogMessage() {}
};

USTRUCT()
struct FConversationOutputData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FConversationLogMessage> LogMessages;
	
	UPROPERTY()
	TArray<FString> Text;
	
	UPROPERTY()
	TArray<FString> NodesVisited;
	
	FConversationOutputData() {}
};

USTRUCT()
struct FConversationInputData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Text;
	
	FConversationInputData() {}
};

USTRUCT()
struct FConversationMessageRequest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FConversationInputData Input;
	
	UPROPERTY()
	bool AlternateIntents;
	
	UPROPERTY()
	FConversationContext Context;
	
	UPROPERTY()
	TArray<FConversationRuntimeEntity> Entities;
	
	UPROPERTY()
	TArray<FConversationRuntimeIntent> Intents;
	
	UPROPERTY()
	FConversationOutputData Output;
	
	FConversationMessageRequest() {}
};

USTRUCT()
struct FConversationMessageInput
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Text;
	
	FConversationMessageInput() {}
};

USTRUCT()
struct FConversationMessageResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FConversationMessageInput Input;
	
	UPROPERTY()
	TArray<FConversationRuntimeIntent> Intents;
	
	UPROPERTY()
	TArray<FConversationRuntimeEntity> Entities;
	
	UPROPERTY()
	bool AlternateIntents;
	
	UPROPERTY()
	FConversationContext Context;
	
	UPROPERTY()
	FConversationOutputData Output;
	
	FConversationMessageResponse() {}
};

USTRUCT()
struct FConversationMessageErrorDetail
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString Message;
	
	UPROPERTY()
	FString Path;
	
	FConversationMessageErrorDetail() {}
};

USTRUCT()
struct FConversationMessageError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString Error;
	
	UPROPERTY()
	TArray<FConversationMessageErrorDetail> Errors;

	FConversationMessageError() {}
};