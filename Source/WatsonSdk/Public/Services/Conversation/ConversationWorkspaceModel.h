#pragma once

#include "ConversationModel.h"
#include "ConversationIntentModel.h"
#include "ConversationEntityModel.h"
#include "ConversationCounterexampleModel.h"
#include "ConversationDialogNodeModel.h"
#include "ConversationWorkspaceModel.generated.h"

USTRUCT()
struct FConversationWorkspace
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FString language;

	TSharedPtr<FJsonObject> metadata;

	UPROPERTY()
	FString created;

	UPROPERTY()
	FString updated;

	UPROPERTY()
	FString workspace_id;

	UPROPERTY()
	FString status;

	UPROPERTY()
	TArray<FConversationIntent> intents;

	UPROPERTY()
	TArray<FConversationEntity> entities;

	UPROPERTY()
	TArray<FConversationCounterexample> counterexamples;

	UPROPERTY()
	TArray<FConversationDialogNode> dialog_nodes;

	UPROPERTY()
	bool learning_opt_out;

	FConversationWorkspace() {}
};

USTRUCT()
struct FConversationWorkspaceList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FConversationWorkspace> workspaces;

	UPROPERTY()
	FConversationPagination pagination;

	FConversationWorkspaceList() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationListWorkspacesSuccess, FConversationWorkspaceList, ConversationWorkspaceList);
USTRUCT()
struct FConversationListWorkspacesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListWorkspacesSuccess OnSuccess;
	FConversationListWorkspacesRequest() {}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationGetWorkspaceSuccess, FConversationWorkspace, ConversationWorkspace);
USTRUCT()
struct FConversationGetWorkspaceRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetWorkspaceSuccess OnSuccess;
	FConversationGetWorkspaceRequest() {}
};


