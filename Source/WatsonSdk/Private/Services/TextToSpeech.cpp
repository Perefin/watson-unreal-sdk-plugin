#pragma once

#include "TextToSpeech.h"
#include "JsonObjectConverter.h"

UTextToSpeech::UTextToSpeech()
{
	SetUrl("https://stream.watsonplatform.net/text-to-speech/api/v1/");
	SetUserAgent("X-UnrealEngine-Agent");
	SetVersion("2017-05-26");
}

//////////////////////////////////////////////////////////////////////////
// Synthesize Audio

FTextToSpeechSynthesizePendingRequest* UTextToSpeech::Synthesize(const FTextToSpeechSynthesizeRequest& Request, const FString& Voice)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(ServiceUrl + "synthesize?voice=" + Voice);
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	HttpRequest->SetContentAsString(StructToString<FTextToSpeechSynthesizeRequest>(Request));
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
	if (DelegatePtr != nullptr)
	{
		TSharedPtr<FTextToSpeechSynthesizePendingRequest> Delegate = *DelegatePtr;
		FString ErrorMessage;
		if (IsRequestSuccessful(Request, Response, bWasSuccessful, ErrorMessage))
		{
			Delegate->Response->audioData = TArray<uint8>(Response->GetContent());
			Delegate->OnSuccess.ExecuteIfBound(Delegate->Response);
		}
		else
		{
			Delegate->OnFailure.ExecuteIfBound(ErrorMessage);
		}
		PendingSynthesisRequests.Remove(Request);
	}
}
