#pragma once

#include "Services/Conversation/Conversation.h"

UConversation::UConversation()
{
	SetUrl("https://gateway.watsonplatform.net/conversation/api/v1/");
	SetUserAgent("X-UnrealEngine-Agent");
	SetVersion("2017-05-26");
}

template<typename RequestType, typename ResponseType>
void UConversation::DefaultOnResponseHandler(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RequestType* CastWatsonRequest = GetWatsonRequest<RequestType>(Request);
	if (CastWatsonRequest != nullptr)
	{
		FString ErrorMessage;
		if (IsRequestSuccessful(Request, Response, bWasSuccessful, ErrorMessage))
		{
			TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
			TSharedPtr<ResponseType> ResponseStruct = JsonObjectToStruct<ResponseType>(ResponseJson);
			CastWatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
		}
		else
		{
			CastWatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
		}
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Send Message

FConversationMessagePendingRequest* UConversation::Message(const FString& WorkspaceId, const FConversationMessageRequest& Message)
{
	TSharedPtr<FJsonObject> MessageJson = StructToJsonObject<FConversationMessageRequest>(Message);

	// Message context is a dynamic object.
	MessageJson->SetObjectField("context", Message.context);

	// Don't serialize empty lists.
	if (Message.intents.Num() == 0)
	{
		MessageJson->RemoveField("intents");
	}
	if (Message.entities.Num() == 0)
	{
		MessageJson->RemoveField("entities");
	}

	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/message";
	FString Query = "?version=" + ServiceVersion;

	TSharedPtr<IHttpRequest> Request = CreateRequest("POST", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->SetContentAsString(JsonObjectToString(MessageJson));
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnMessage);
	return NewWatsonRequest<FConversationMessagePendingRequest>(Request);
}

void UConversation::OnMessage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FConversationMessagePendingRequest* CastWatsonRequest = GetWatsonRequest<FConversationMessagePendingRequest>(Request);
	if (CastWatsonRequest != nullptr)
	{
		FString ErrorMessage;
		if (IsRequestSuccessful(Request, Response, bWasSuccessful, ErrorMessage))
		{
			TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
			TSharedPtr<FConversationMessageResponse> ResponseStruct = JsonObjectToStruct<FConversationMessageResponse>(ResponseJson);
			// Message context is a dynamic object.
			ResponseStruct->context = ResponseJson->GetObjectField("context");
			CastWatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
		}
		else
		{
			CastWatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
		}
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Workspaces

FConversationListWorkspacesRequest* UConversation::ListWorkspaces()
{
	FString Path = ServiceUrl + "workspaces";
	FString Query = "?version=" + ServiceVersion;

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListWorkspaces);
	return NewWatsonRequest<FConversationListWorkspacesRequest>(Request);
}

void UConversation::OnListWorkspaces(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationListWorkspacesRequest, FConversationWorkspaceList>(Request, Response, bWasSuccessful);
}

//////////////////////////////////////////////////////////////////////////
// Get Workspace

FConversationGetWorkspaceRequest* UConversation::GetWorkspace(const FString& WorkspaceId, bool DoExport)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId;
	FString Query = "?version=" + ServiceVersion;
	Query += (DoExport ? "&export=true" : "&export=false");

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetWorkspace);
	return NewWatsonRequest<FConversationGetWorkspaceRequest>(Request);
}

void UConversation::OnGetWorkspace(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationGetWorkspaceRequest, FConversationWorkspace>(Request, Response, bWasSuccessful);
}

//////////////////////////////////////////////////////////////////////////
// List Counterexamples

FConversationListCounterexamplesRequest* UConversation::ListCounterexamples(const FString& WorkspaceId, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/counterexamples";
	FString Query = "?version=" + ServiceVersion;
	Query += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Query += ("&page_limit=" + FString::FromInt(PageLimit));
	Query += ("&sort=" + Sort);

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListCounterexamples);
	return NewWatsonRequest<FConversationListCounterexamplesRequest>(Request);
}

void UConversation::OnListCounterexamples(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationListCounterexamplesRequest, FConversationCounterexampleList>(Request, Response, bWasSuccessful);
}

//////////////////////////////////////////////////////////////////////////
// Get Counterexample

FConversationGetCounterexampleRequest* UConversation::GetCounterexample(const FString& WorkspaceId, const FString& UrlEncodedText)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/counterexamples/" + UrlEncodedText;
	FString Query = "?version=" + ServiceVersion;

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetCounterexample);
	return NewWatsonRequest<FConversationGetCounterexampleRequest>(Request);
}

void UConversation::OnGetCounterexample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationGetCounterexampleRequest, FConversationCounterexample>(Request, Response, bWasSuccessful);
}

//////////////////////////////////////////////////////////////////////////
// List Entities

FConversationListEntitiesRequest* UConversation::ListEntities(const FString& WorkspaceId, bool DoExport, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities";
	FString Query = "?version=" + ServiceVersion;
	Query += (DoExport ? "&export=true" : "&export=false");
	Query += ("&page_limit=" + FString::FromInt(PageLimit));
	Query += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Query += ("&sort=" + Sort);

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListEntities);
	return NewWatsonRequest<FConversationListEntitiesRequest>(Request);
}

void UConversation::OnListEntities(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationListEntitiesRequest, FConversationEntityList>(Request, Response, bWasSuccessful);
}

//////////////////////////////////////////////////////////////////////////
// Get Entity

FConversationGetEntityRequest* UConversation::GetEntity(const FString& WorkspaceId, const FString& Entity, bool DoExport)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity;
	FString Query = "?version=" + ServiceVersion;
	Query += (DoExport ? "&export=true" : "&export=false");

	TSharedPtr<IHttpRequest> Request = CreateRequest("GET", Path + Query);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListCounterexamples);
	return NewWatsonRequest<FConversationGetEntityRequest>(Request);
}

void UConversation::OnGetEntity(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	DefaultOnResponseHandler<FConversationGetEntityRequest, FConversationEntity>(Request, Response, bWasSuccessful);
}