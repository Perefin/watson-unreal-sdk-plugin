#pragma once

#include "Services/SpeechToText/SpeechToText.h"
#include "JsonObjectConverter.h"

USpeechToText::USpeechToText()
{
	Configuration.Url = "https://stream.watsonplatform.net/speech-to-text/api/v1/";
	Configuration.UserAgent = "X-UnrealEngine-Agent";
	Configuration.Version = "2017-05-26";
}

//////////////////////////////////////////////////////////////////////////
// Sessionless Recognize Audio

FSpeechToTextRecognizePendingRequest* USpeechToText::Recognize(TArray<uint8> AudioData, const FString& AudioModel)
{
	TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "recognize?model=" + AudioModel);
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.Encode());
	HttpRequest->SetContent(AudioData);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &USpeechToText::OnRecognizeComplete);

	TSharedPtr<FSpeechToTextRecognizePendingRequest> Delegate = MakeShareable(new FSpeechToTextRecognizePendingRequest);
	PendingRecognizeRequests.Add(HttpRequest, Delegate);
	Delegate->HttpRequest = HttpRequest;
	return Delegate.Get();
}

void USpeechToText::OnRecognizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FSpeechToTextRecognizePendingRequest>* DelegatePtr = PendingRecognizeRequests.Find(Request);
	if (DelegatePtr == nullptr)
	{
		return;
	}

	TSharedPtr<FSpeechToTextRecognizePendingRequest> Delegate = *DelegatePtr;
	if (!bWasSuccessful)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request not successful."));
		PendingRecognizeRequests.Remove(Request);
		return;
	}

	if (Response->GetResponseCode() != 200)
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Request failed: ") + Response->GetContentAsString());
		PendingRecognizeRequests.Remove(Request);
		return;
	}

	TSharedPtr<FSpeechToTextRecognizeResponse> RecognizeResponse = MakeShareable(new FSpeechToTextRecognizeResponse);
	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FSpeechToTextRecognizeResponse>(Response->GetContentAsString(), RecognizeResponse.Get(), 0, 0))
	{
		Delegate->OnFailure.ExecuteIfBound(FString("Could not deserialize: ") + Response->GetContentAsString());
		PendingRecognizeRequests.Remove(Request);
		return;
	}

	Delegate->OnSuccess.ExecuteIfBound(RecognizeResponse);
	PendingRecognizeRequests.Remove(Request);
}

