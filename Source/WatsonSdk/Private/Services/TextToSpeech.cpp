#pragma once

#include "TextToSpeech.h"
#include "JsonObjectConverter.h"

UTextToSpeech::UTextToSpeech()
{
	Configuration.Url = "https://stream.watsonplatform.net/text-to-speech/api/v1/";
	Configuration.UserAgent = "X-UnrealEngine-Agent";
	Configuration.Version = "2017-05-26";
}

//////////////////////////////////////////////////////////////////////////
// Synthesize Audio

FTextToSpeechSynthesizePendingRequest* UTextToSpeech::Synthesize(const FTextToSpeechSynthesizeRequest& Request, const FString& Voice)
{
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(FTextToSpeechSynthesizeRequest::StaticStruct(), &Request, Content, 0, 0);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "synthesize?voice=" + Voice);
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.Encode());
	HttpRequest->SetContentAsString(Content);
	HttpRequest->OnRequestProgress().BindUObject(this, &UTextToSpeech::OnSynthesizeProgress);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UTextToSpeech::OnSynthesizeComplete);

	TSharedPtr<FTextToSpeechSynthesizePendingRequest> Delegate = MakeShareable(new FTextToSpeechSynthesizePendingRequest);
	PendingSynthesisRequests.Add(HttpRequest, Delegate);
	Delegate->HttpRequest = HttpRequest;
	Delegate->Response = MakeShareable(new FTextToSpeechSynthesizeResponse);
	return Delegate.Get();
}

void UTextToSpeech::OnSynthesizeProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	TSharedPtr<FTextToSpeechSynthesizePendingRequest>* DelegatePtr = PendingSynthesisRequests.Find(Request);
	if (DelegatePtr != nullptr)
	{
		TSharedPtr<FTextToSpeechSynthesizePendingRequest> Delegate = *DelegatePtr;
		Delegate->Response->audioLength = BytesReceived;
	}	
}

void UTextToSpeech::OnSynthesizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FTextToSpeechSynthesizePendingRequest>* DelegatePtr = PendingSynthesisRequests.Find(Request);
	if (DelegatePtr == nullptr)
	{
		return;
	}

	TSharedPtr<FTextToSpeechSynthesizePendingRequest> Delegate = *DelegatePtr;
	if (!bWasSuccessful)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request not successful."));
		PendingSynthesisRequests.Remove(Request);
		return;
	}

	if (Response->GetResponseCode() != 200)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request failed: ") + Response->GetContentAsString());
		PendingSynthesisRequests.Remove(Request);
		return;
	}

	Delegate->Response->audioData = TArray<uint8>(Response->GetContent());
	Delegate->OnSuccess.ExecuteIfBound(Delegate->Response);
	PendingSynthesisRequests.Remove(Request);
}
