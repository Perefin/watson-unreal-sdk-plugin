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

TSharedPtr<FTextToSpeechSynthesizeDelegate> UTextToSpeech::Synthesize(const FSynthesizeRequest& Request)
{
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(FSynthesizeRequest::StaticStruct(), &Request, Content, 0, 0);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Configuration.Url + "synthesize?voice=en-US_MichaelVoice");
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("audio/l16;rate=16000;channels=1;"));
	HttpRequest->SetHeader(TEXT("User-Agent"), Configuration.UserAgent);
	HttpRequest->SetHeader(TEXT("Content-Type"), "application/json");
	HttpRequest->SetHeader(TEXT("Authorization"), Authorization.encode());
	HttpRequest->SetContentAsString(Content);

	TSharedPtr<FTextToSpeechSynthesizeDelegate> Delegate = MakeShareable(new FTextToSpeechSynthesizeDelegate);
	TSharedPtr<FSynthesisProgress> Progress = MakeShareable(new FSynthesisProgress(Delegate));
	PendingSynthesisRequests.Add(HttpRequest, Progress);
	HttpRequest->OnRequestProgress().BindUObject(this, &UTextToSpeech::OnSynthesizeProgress);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UTextToSpeech::OnSynthesizeComplete);
	HttpRequest->ProcessRequest();
	return Delegate;
}

void UTextToSpeech::OnSynthesizeProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	TSharedPtr<FSynthesisProgress>* ProgressPtr = PendingSynthesisRequests.Find(Request);
	if (ProgressPtr != nullptr)
	{
		TSharedPtr<FSynthesisProgress> Progress = *ProgressPtr;
		TSharedPtr<FSynthesizeResponse> SynthesisResponse = Progress->Response;
		SynthesisResponse->audioLength = BytesReceived;
	}	
}

void UTextToSpeech::OnSynthesizeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FSynthesisProgress>* ProgressPtr = PendingSynthesisRequests.Find(Request);
	if (ProgressPtr != nullptr)
	{
		TSharedPtr<FSynthesisProgress> Progress = *ProgressPtr;
		TSharedPtr<FSynthesizeResponse> SynthesisResponse = Progress->Response;
		SynthesisResponse->audioData = TArray<uint8>(Response->GetContent());
		Progress->Delegate.Get()->ExecuteIfBound(SynthesisResponse, nullptr);
		PendingSynthesisRequests.Remove(Request);
	}
}
