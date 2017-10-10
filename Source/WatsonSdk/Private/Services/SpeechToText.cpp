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

TSharedPtr<FSpeechToTextRecognizeDelegate> USpeechToText::Recognize(TArray<uint8> AudioData, const FString& AudioModel)
{
	TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "recognize?model=" + AudioModel);
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.encode());
	HttpRequest->SetContent(AudioData);

	TSharedPtr<FSpeechToTextRecognizeDelegate> Delegate = MakeShareable(new FSpeechToTextRecognizeDelegate);
	PendingRecognizeRequests.Add(HttpRequest, Delegate);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &USpeechToText::OnRecognizeComplete);
	HttpRequest->ProcessRequest();
	return Delegate;
}

void USpeechToText::OnRecognizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FSpeechToTextRecognizeDelegate>* DelegatePtr = PendingRecognizeRequests.Find(Request);
	if (DelegatePtr != nullptr)
	{
		TSharedPtr<FSpeechRecognitionEvent> RecognizeResponse = MakeShareable(new FSpeechRecognitionEvent);
		FString SerializedResponseBody(Response->GetContentAsString());
		FJsonObjectConverter::JsonObjectStringToUStruct<FSpeechRecognitionEvent>(SerializedResponseBody, RecognizeResponse.Get(), 0, 0);

		TSharedPtr<FSpeechToTextRecognizeDelegate> Delegate = *DelegatePtr;
		Delegate.Get()->ExecuteIfBound(RecognizeResponse, nullptr);
		PendingRecognizeRequests.Remove(Request);
	}
}

