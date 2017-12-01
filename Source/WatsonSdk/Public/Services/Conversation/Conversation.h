#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "ConversationModel.h"
#include "ConversationCounterexampleModel.h"
#include "ConversationDialogNodeModel.h"
#include "ConversationEntityModel.h"
#include "ConversationExampleModel.h"
#include "ConversationIntentModel.h"
#include "ConversationSynonymModel.h"
#include "ConversationValueModel.h"
#include "ConversationWorkspaceModel.h"
#include "ConversationMessageModel.h"
#include "Conversation.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Watson")
class WATSONSDK_API UConversation : public UWatsonService
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
	UConversation();

	FConversationMessagePendingRequest* Message(const FString& WorkspaceId, const FConversationMessageRequest& Message);
	FConversationListWorkspacesRequest* ListWorkspaces();
	FConversationGetWorkspaceRequest* GetWorkspace(const FString& WorkspaceId, bool DoExport = false);
	FConversationListCounterexamplesRequest* ListCounterexamples(const FString& WorkspaceId, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	FConversationGetCounterexampleRequest* GetCounterexample(const FString& WorkspaceId, const FString& UrlEncodedText);
	FConversationListEntitiesRequest* ListEntities(const FString& WorkspaceId, bool DoExport = false, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	FConversationGetEntityRequest* GetEntity(const FString& WorkspaceId, const FString& Entity, bool DoExport = false);
	FConversationListValuesRequest* ListValues(const FString& WorkspaceId, const FString& Entity, bool DoExport = false, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	FConversationGetValueRequest* GetValue(const FString& WorkspaceId, const FString& Entity, const FString& Value, bool DoExport = false);
	FConversationListSynonymsRequest* ListSynonyms(const FString& WorkspaceId, const FString& Entity, const FString& Value, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	FConversationGetSynonymRequest* GetSynonym(const FString& WorkspaceId, const FString& Entity, const FString& Value, const FString& Synonym);
	//FConversationListIntentsRequest* ListIntents(const FString& WorkspaceId, bool DoExport = false, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	//FConversationGetIntentRequest* GetIntent(const FString& WorkspaceId, const FString& Intent, bool DoExport = false);
	//FConversationListExamplesRequest* ListExamples(const FString& WorkspaceId, const FString& Intent, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	//FConversationGetExampleRequest* GetExample(const FString& WorkspaceId, const FString& Intent, const FString& Text);
	//FConversationListDialogNodesRequest* ListDialogNodes(const FString& WorkspaceId, int32 PageLimit = 100, bool IncludeCount = false, const FString& Sort = "updated");
	//FConversationGetDialogNodeRequest* GetDialogNode(const FString& WorkspaceId, const FString& DialogNode);

private:
	void OnMessage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListWorkspaces(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetWorkspace(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListCounterexamples(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetCounterexample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListEntities(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetEntity(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListValues(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetValue(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListSynonyms(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetSynonym(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnListIntents(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnGetIntent(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnListExamples(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnGetExample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnListDialogNodes(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void OnGetDialogNode(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
