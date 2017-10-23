#pragma once

#include "Services/Conversation/Conversation.h"
#include "JsonObjectConverter.h"

UConversation::UConversation()
{
	SetUrl("https://gateway.watsonplatform.net/conversation/api/v1/");
	SetUserAgent("X-UnrealEngine-Agent");
	SetVersion("2017-05-26");
}

//////////////////////////////////////////////////////////////////////////
// Send Message

FConversationMessagePendingRequest* UConversation::Message(const FString& Workspace, const FConversationMessageRequest& Message)
{
	// Message context is a dynamic object.
	TSharedPtr<FJsonObject> MessageJson = StructToJsonObject<FConversationMessageRequest>(Message);
	MessageJson->SetObjectField("context", Message.context);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(ServiceUrl + "workspaces/" + Workspace + "/message?version=" + ServiceVersion);
	HttpRequest->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	HttpRequest->SetContentAsString(JsonObjectToString(MessageJson));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UConversation::OnMessageComplete);

	TSharedPtr<FConversationMessagePendingRequest> PendingRequest = MakeShareable(new FConversationMessagePendingRequest);
	PendingMessageRequests.Add(HttpRequest, PendingRequest);
	PendingRequest->HttpRequest = HttpRequest;
	return PendingRequest.Get();
}

void UConversation::OnMessageComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FConversationMessagePendingRequest>* DelegatePtr = PendingMessageRequests.Find(Request);
	if (DelegatePtr == nullptr)
	{
		return;
	}

	TSharedPtr<FConversationMessagePendingRequest> Delegate = *DelegatePtr;
	if (!bWasSuccessful)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request not successful."));
		PendingMessageRequests.Remove(Request);
		return;
	}

	if (Response->GetResponseCode() != 200)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request failed: ") + Response->GetContentAsString());
		PendingMessageRequests.Remove(Request);
		return;
	}

	TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
	TSharedPtr<FConversationMessageResponse> ResponseStruct = JsonObjectToStruct<FConversationMessageResponse>(ResponseJson);
	ResponseStruct->context = ResponseJson->GetObjectField("context");
	Delegate->OnSuccess.ExecuteIfBound(ResponseStruct);
	PendingMessageRequests.Remove(Request);
}