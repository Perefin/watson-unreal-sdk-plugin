#pragma once

#include "Services/SpeechToText/SpeechToText.h"

USpeechToText::USpeechToText()
{
	SetUrl("https://stream.watsonplatform.net/speech-to-text/api/v1/");
	SetUserAgent("X-UnrealEngine-Agent");
	SetVersion("2017-05-26");
}

//////////////////////////////////////////////////////////////////////////
// Sessionless Recognize Audio

FSpeechToTextRecognizePendingRequest* USpeechToText::Recognize(TArray<uint8> AudioData, const FString& AudioModel)
{
	TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(ServiceUrl + "recognize?model=" + AudioModel);
	HttpRequest->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
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
	if (DelegatePtr != nullptr)
	{
		TSharedPtr<FSpeechToTextRecognizePendingRequest> Delegate = *DelegatePtr;
		FString ErrorMessage;
		if (IsRequestSuccessful(Request, Response, bWasSuccessful, ErrorMessage))
		{
			TSharedPtr<FSpeechToTextRecognizeResponse> ResponseStruct = StringToStruct<FSpeechToTextRecognizeResponse>(Response->GetContentAsString());
			Delegate->OnSuccess.ExecuteIfBound(ResponseStruct);
		}
		else
		{
			Delegate->OnFailure.ExecuteIfBound(ErrorMessage);
		}
		PendingRecognizeRequests.Remove(Request);
	}
}