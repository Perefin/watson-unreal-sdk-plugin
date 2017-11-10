#pragma once

#include "ConversationModel.h"
#include "ConversationValueModel.h"
#include "ConversationEntityModel.generated.h"

USTRUCT()
struct FConversationEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString entity;

	UPROPERTY()
	FString created;

	UPROPERTY()
	FString updated;

	UPROPERTY()
	FString description;

	TSharedPtr<FJsonObject> metadata;

	UPROPERTY()
	bool fuzzy_match;

	UPROPERTY()
	TArray<FConversationValue> values;

	FConversationEntity() {}
};

USTRUCT()
struct FConversationEntityList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FConversationEntity> entities;

	UPROPERTY()
	FConversationPagination pagination;

	FConversationEntityList() {}
};


DECLARE_DELEGATE_OneParam(FConversationListEntitiesSuccess, TSharedPtr<FConversationEntityList>)
USTRUCT()
struct FConversationListEntitiesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListEntitiesSuccess OnSuccess;
	FConversationListEntitiesRequest() {}
};


DECLARE_DELEGATE_OneParam(FConversationGetEntitySuccess, TSharedPtr<FConversationEntity>)
USTRUCT()
struct FConversationGetEntityRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetEntitySuccess OnSuccess;
	FConversationGetEntityRequest() {}
};