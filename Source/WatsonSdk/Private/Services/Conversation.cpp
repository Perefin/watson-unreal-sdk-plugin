#pragma once

#include "Services/Conversation/Conversation.h"
#include "JsonObjectConverter.h"

UConversation::UConversation()
{
	Configuration.Url = "https://gateway.watsonplatform.net/conversation/api/v1/";
	Configuration.UserAgent = "X-UnrealEngine-Agent";
	Configuration.Version = "2017-05-26";
}

//////////////////////////////////////////////////////////////////////////
// Send Message

FConversationMessagePendingRequest* UConversation::Message(const FString& Workspace, const FConversationMessageRequest& Message)
{
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(FConversationMessageRequest::StaticStruct(), &Message, Content, 0, 0);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "workspaces/" + Workspace + "/message?version=" + Configuration.Version);
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.Encode());
	HttpRequest->SetContentAsString(Content);
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
	
	TSharedPtr<FConversationMessageResponse> MessageResponse = MakeShareable(new FConversationMessageResponse);
	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FConversationMessageResponse>(Response->GetContentAsString(), MessageResponse.Get(), 0, 0))
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Could not deserialize: ") + Response->GetContentAsString());
		PendingMessageRequests.Remove(Request);
		return;
	}

	Delegate->OnSuccess.ExecuteIfBound(MessageResponse);
	PendingMessageRequests.Remove(Request);
}