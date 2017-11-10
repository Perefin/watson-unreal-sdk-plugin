#pragma once

#include "ConversationModel.generated.h"

USTRUCT()
struct FConversationErrorDetail
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString message;

	UPROPERTY()
	FString path;

	FConversationErrorDetail() {}
};

USTRUCT()
struct FConversationError
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString error;

	UPROPERTY()
	TArray<FConversationErrorDetail> errors;

	FConversationError() {}
};

USTRUCT()
struct FConversationPagination
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString refresh_url;

	UPROPERTY()
	FString next_url;

	UPROPERTY()
	int32 total;

	UPROPERTY()
	int32 matched;

	FConversationPagination() {}
};

USTRUCT()
struct FConversationEmptyResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString stub; // Just to avoid an empty struct.

	FConversationEmptyResponse() {}
};