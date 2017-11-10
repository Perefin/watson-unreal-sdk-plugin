#pragma once

#include "ConversationModel.h"
#include "ConversationDialogNodeModel.generated.h"

USTRUCT()
struct FConversationDialogNodeAction
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString name;

    UPROPERTY()
    FString type;

    TSharedPtr<FJsonObject> parameters;

    UPROPERTY()
    FString result_variable;

    FConversationDialogNodeAction() {}
};

USTRUCT()
struct FConversationDialogNodeNextStep
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString behavior;

    UPROPERTY()
    FString dialog_node;

    UPROPERTY()
    FString selector;

    FConversationDialogNodeNextStep() {}
};

USTRUCT()
struct FConversationDialogNode
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString dialog_node;

    UPROPERTY()
    FString description;

    UPROPERTY()
    FString conditions;

    UPROPERTY()
    FString parent;

    UPROPERTY()
    FString previous_sibling;

    TSharedPtr<FJsonObject> output;

    TSharedPtr<FJsonObject> context;

    TSharedPtr<FJsonObject> metadata;

    UPROPERTY()
    FConversationDialogNodeNextStep next_step;

    UPROPERTY()
    FString created;

    UPROPERTY()
    FString updated;

    UPROPERTY()
    TArray<FConversationDialogNodeAction> actions;

    UPROPERTY()
    FString title;

    UPROPERTY()
    FString type;

    UPROPERTY()
    FString event_name;

    UPROPERTY()
    FString variable;

    FConversationDialogNode() {}
};

USTRUCT()
struct FConversationDialogNodeList
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FConversationDialogNode> dialog_nodes;

    UPROPERTY()
    FConversationPagination pagination;

    FConversationDialogNodeList() {}
};


DECLARE_DELEGATE_OneParam(FConversationListDialogNodesSuccess, TSharedPtr<FConversationDialogNodeList>)
USTRUCT()
struct FConversationListDialogNodesRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationListDialogNodesSuccess OnSuccess;
	FConversationListDialogNodesRequest() {}
};


DECLARE_DELEGATE_OneParam(FConversationGetDialogNodeSuccess, TSharedPtr<FConversationDialogNode>)
USTRUCT()
struct FConversationGetDialogNodeRequest : public FWatsonRequest
{
	GENERATED_USTRUCT_BODY()
	FConversationGetDialogNodeSuccess OnSuccess;
	FConversationGetDialogNodeRequest() {}
};