#pragma once

#include "MessageDataModel.generated.h"

USTRUCT()
struct FConversationMessageRuntimeIntent
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString intent;
	
	UPROPERTY()
	float confidence;
	
	FConversationMessageRuntimeIntent() {}
};

USTRUCT()
struct FConversationMessageRuntimeEntity
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString entity;
	
	UPROPERTY()
	TArray<int32> location;
	
	UPROPERTY()
	FString value;
	
	UPROPERTY()
	float confidence;
	
	FConversationMessageRuntimeEntity() {}
};

USTRUCT()
struct FConversationMessageLogMessage
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString level;
	
	UPROPERTY()
	FString msg;
	
	FConversationMessageLogMessage() {}
};

USTRUCT()
struct FConversationMessageOutputData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<FConversationMessageLogMessage> log_messages;
	
	UPROPERTY()
	TArray<FString> text;
	
	UPROPERTY()
	TArray<FString> nodes_visited;
	
	FConversationMessageOutputData() {}
};

USTRUCT()
struct FConversationMessageInputData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString text;
	
	FConversationMessageInputData() {}
};

USTRUCT()
struct FConversationMessageRequest
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FConversationMessageInputData input;
	
	UPROPERTY()
	bool alternate_intents;
	
	UPROPERTY()
	TArray<FConversationMessageRuntimeEntity> entities;
	
	UPROPERTY()
	TArray<FConversationMessageRuntimeIntent> intents;
	
	UPROPERTY()
	FConversationMessageOutputData output;

	TSharedPtr<FJsonObject> context;

	FConversationMessageRequest() {}
};

USTRUCT()
struct FConversationMessageInput
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString text;
	
	FConversationMessageInput() {}
};

USTRUCT()
struct FConversationMessageResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FConversationMessageInput input;
	
	UPROPERTY()
	TArray<FConversationMessageRuntimeIntent> intents;
	
	UPROPERTY()
	TArray<FConversationMessageRuntimeEntity> entities;
	
	UPROPERTY()
	bool alternate_intents;
	
	UPROPERTY()
	FConversationMessageOutputData output;

	TSharedPtr<FJsonObject> context;
	
	FConversationMessageResponse() {}
};

USTRUCT()
struct FConversationMessageErrorDetail
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString message;
	
	UPROPERTY()
	FString path;
	
	FConversationMessageErrorDetail() {}
};

USTRUCT()
struct FConversationMessageError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString error;
	
	UPROPERTY()
	TArray<FConversationMessageErrorDetail> errors;
	
	FConversationMessageError() {}
};

DECLARE_DELEGATE_OneParam(FConversationMessageSuccessDelegate, TSharedPtr<FConversationMessageResponse>)
DECLARE_DELEGATE_OneParam(FConversationMessageFailureDelegate, FString)
struct FConversationMessagePendingRequest
{
	TSharedPtr<IHttpRequest> HttpRequest;
	FConversationMessageSuccessDelegate OnSuccess;
	FConversationMessageFailureDelegate OnFailure;
	void Send()
	{
		HttpRequest->ProcessRequest();
	}
};