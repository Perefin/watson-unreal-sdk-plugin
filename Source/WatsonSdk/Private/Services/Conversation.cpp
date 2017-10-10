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

TSharedPtr<FConversationMessageDelegate> UConversation::Message(const FString& Workspace, const FConversationMessageRequest& Message)
{
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(FConversationMessageRequest::StaticStruct(), &Message, Content, 0, 0);

	TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "workspaces/" + Workspace + "/message?version=" + Configuration.Version);
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.encode());
	HttpRequest->SetContentAsString(Content);

	TSharedPtr<FConversationMessageDelegate> Delegate = MakeShareable(new FConversationMessageDelegate);
	PendingMessageRequests.Add(HttpRequest, Delegate);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UConversation::OnMessageComplete);
	HttpRequest->ProcessRequest();
	return Delegate;
}

void UConversation::OnMessageComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FConversationMessageDelegate>* DelegatePtr = PendingMessageRequests.Find(Request);
	if (DelegatePtr != nullptr)
	{
		TSharedPtr<FConversationMessageResponse> MessageResponse = MakeShareable(new FConversationMessageResponse);
		FString SerializedResponseBody(Response->GetContentAsString());
		FJsonObjectConverter::JsonObjectStringToUStruct<FConversationMessageResponse>(SerializedResponseBody, MessageResponse.Get(), 0, 0);

		TSharedPtr<FConversationMessageDelegate> Delegate = *DelegatePtr;
		Delegate.Get()->ExecuteIfBound(MessageResponse, nullptr);
		PendingMessageRequests.Remove(Request);
	}
}