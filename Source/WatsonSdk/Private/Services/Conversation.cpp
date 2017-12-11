#pragma once

#include "Services/Conversation/Conversation.h"

UConversation::UConversation()
{
	SetUrl("https://gateway.watsonplatform.net/conversation/api/v1/");
	SetUserAgent("X-UnrealEngine-Agent");
	SetVersion("2017-05-26");
}

//////////////////////////////////////////////////////////////////////////
// Send Message

FConversationMessagePendingRequest* UConversation::Message(const FString& WorkspaceId, const FConversationMessageRequest& Message)
{
	TSharedPtr<FJsonObject> MessageJson = StructToJsonObject<FConversationMessageRequest>(Message);
	MessageJson->SetObjectField("context", Message.context);
	RemoveJsonArrayIfEmpty(MessageJson.Get(), "intents", Message.intents);
	RemoveJsonArrayIfEmpty(MessageJson.Get(), "entities", Message.entities);

	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/message";
	Path += ("?version=" + ServiceVersion);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->SetContentAsString(JsonObjectToString(MessageJson));
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnMessage);
	return CreateWatsonRequest<FConversationMessagePendingRequest>(Request);
}

void UConversation::MakeConversationRequest(const FString& WorkspaceId, const FConversationMessageRequest& Message, FConversationMessageSuccess OnSuccess, FWatsonRequestFailure OnFailure)
{
	FConversationMessagePendingRequest* Request = UConversation::Message(WorkspaceId, Message);
	Request->OnSuccess = OnSuccess;
	Request->OnFailure = OnFailure;
	Request->Send();
}

FConversationMessageRequest UConversation::PassContextAndOutputToRequest(FConversationMessageRequest Request, FConversationMessageResponse Response)
{
	Request.context = Response.context;
	Request.output = Response.output;

	return Request;
}

void UConversation::OnMessage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationMessagePendingRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationMessageResponse ResponseStruct = JsonObjectToStruct<FConversationMessageResponse>(ResponseJson);
		ResponseStruct.context = ResponseJson->GetObjectField("context");
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Workspaces

FConversationListWorkspacesRequest* UConversation::ListWorkspaces()
{
	FString Path = ServiceUrl + "workspaces";
	Path += ("?version=" + ServiceVersion);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListWorkspaces);
	return CreateWatsonRequest<FConversationListWorkspacesRequest>(Request);
}

void UConversation::OnListWorkspaces(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationListWorkspacesRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationWorkspaceList ResponseStruct = JsonObjectToStruct<FConversationWorkspaceList>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Get Workspace

FConversationGetWorkspaceRequest* UConversation::GetWorkspace(const FString& WorkspaceId, bool DoExport)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId;
	Path += ("?version=" + ServiceVersion);
	Path += (DoExport ? "&export=true" : "&export=false");

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetWorkspace);
	return CreateWatsonRequest<FConversationGetWorkspaceRequest>(Request);
}

void UConversation::OnGetWorkspace(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationGetWorkspaceRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationWorkspace ResponseStruct = JsonObjectToStruct<FConversationWorkspace>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Counterexamples

FConversationListCounterexamplesRequest* UConversation::ListCounterexamples(const FString& WorkspaceId, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/counterexamples";
	Path += ("?version=" + ServiceVersion);
	Path += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Path += ("&page_limit=" + FString::FromInt(PageLimit));
	Path += ("&sort=" + Sort);
	
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListCounterexamples);
	return CreateWatsonRequest<FConversationListCounterexamplesRequest>(Request);
}

void UConversation::OnListCounterexamples(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationListCounterexamplesRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationCounterexampleList ResponseStruct = JsonObjectToStruct<FConversationCounterexampleList>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Get Counterexample

FConversationGetCounterexampleRequest* UConversation::GetCounterexample(const FString& WorkspaceId, const FString& UrlEncodedText)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/counterexamples/" + UrlEncodedText;
	Path += ("?version=" + ServiceVersion);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetCounterexample);
	return CreateWatsonRequest<FConversationGetCounterexampleRequest>(Request);
}

void UConversation::OnGetCounterexample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationGetCounterexampleRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationCounterexample ResponseStruct = JsonObjectToStruct<FConversationCounterexample>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Entities

FConversationListEntitiesRequest* UConversation::ListEntities(const FString& WorkspaceId, bool DoExport, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities";
	Path += ("?version=" + ServiceVersion);
	Path += (DoExport ? "&export=true" : "&export=false");
	Path += ("&page_limit=" + FString::FromInt(PageLimit));
	Path += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Path += ("&sort=" + Sort);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListEntities);
	return CreateWatsonRequest<FConversationListEntitiesRequest>(Request);
}

void UConversation::OnListEntities(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationListEntitiesRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationEntityList ResponseStruct = JsonObjectToStruct<FConversationEntityList>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Get Entity

FConversationGetEntityRequest* UConversation::GetEntity(const FString& WorkspaceId, const FString& Entity, bool DoExport)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity;
	Path += ("?version=" + ServiceVersion);
	Path += (DoExport ? "&export=true" : "&export=false");

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListCounterexamples);
	return CreateWatsonRequest<FConversationGetEntityRequest>(Request);
}

void UConversation::OnGetEntity(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationGetEntityRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationEntity ResponseStruct = JsonObjectToStruct<FConversationEntity>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Values

FConversationListValuesRequest* UConversation::ListValues(const FString& WorkspaceId, const FString& Entity, bool DoExport, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity + "/values";
	Path += ("?version=" + ServiceVersion);
	Path += (DoExport ? "&export=true" : "&export=false");
	Path += ("&page_limit=" + FString::FromInt(PageLimit));
	Path += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Path += ("&sort=" + Sort);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListValues);
	return CreateWatsonRequest<FConversationListValuesRequest>(Request);
}

void UConversation::OnListValues(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationListValuesRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationValueList ResponseStruct = JsonObjectToStruct<FConversationValueList>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Get Value

FConversationGetValueRequest* UConversation::GetValue(const FString& WorkspaceId, const FString& Entity, const FString& Value, bool DoExport)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity + "/values/" + Value;
	Path += ("?version=" + ServiceVersion);
	Path += (DoExport ? "&export=true" : "&export=false");

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetValue);
	return CreateWatsonRequest<FConversationGetValueRequest>(Request);
}

void UConversation::OnGetValue(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationGetValueRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationValue ResponseStruct = JsonObjectToStruct<FConversationValue>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// List Synonyms

FConversationListSynonymsRequest* UConversation::ListSynonyms(const FString& WorkspaceId, const FString& Entity, const FString& Value, int32 PageLimit, bool IncludeCount, const FString& Sort)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity + "/values/" + Value + "/synonyms";
	Path += ("?version=" + ServiceVersion);
	Path += ("&page_limit=" + FString::FromInt(PageLimit));
	Path += (IncludeCount ? "&include_count=true" : "&include_count=false");
	Path += ("&sort=" + Sort);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnListSynonyms);
	return CreateWatsonRequest<FConversationListSynonymsRequest>(Request);
}

void UConversation::OnListSynonyms(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationListSynonymsRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationSynonymList ResponseStruct = JsonObjectToStruct<FConversationSynonymList>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}

//////////////////////////////////////////////////////////////////////////
// Get Synonym

FConversationGetSynonymRequest* UConversation::GetSynonym(const FString& WorkspaceId, const FString& Entity, const FString& Value, const FString& Synonym)
{
	FString Path = ServiceUrl + "workspaces/" + WorkspaceId + "/entities/" + Entity + "/values/" + Value + "/synonyms/" + Synonym;
	Path += ("?version=" + ServiceVersion);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->OnProcessRequestComplete().BindUObject(this, &UConversation::OnGetSynonym);
	return CreateWatsonRequest<FConversationGetSynonymRequest>(Request);
}

void UConversation::OnGetSynonym(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FConversationGetSynonymRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FConversationSynonym ResponseStruct = JsonObjectToStruct<FConversationSynonym>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}