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

FTextToSpeechSynthesizeAudioRequest* UTextToSpeech::SynthesizeAudio(const FString& Text, const FString& Voice, const FString& CustomizationId, const FString& Accept)
{
	TSharedPtr<FJsonObject> MessageJson = MakeShareable(new FJsonObject());
	MessageJson->SetStringField("text", Text);

	FString Path = ServiceUrl + "synthesize";
	Path += ("?version=" + ServiceVersion);
	Path += ("&voice=" + Voice);
	Path += ("&accept=" + Accept);
	Path += (CustomizationId.IsEmpty() ? "" : "&customization_id=" + CustomizationId);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), "application/json");
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->SetContentAsString(JsonObjectToString(MessageJson));
	Request->OnProcessRequestComplete().BindUObject(this, &UTextToSpeech::OnSynthesizeAudio);
	Request->OnRequestProgress().BindUObject(this, &UTextToSpeech::OnSynthesizeAudioProgress);
	return CreateWatsonRequest<FTextToSpeechSynthesizeAudioRequest>(Request);
}

void UTextToSpeech::OnSynthesizeAudioProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FTextToSpeechSynthesizeAudioRequest* WatsonRequest = RetrieveWatsonRequest<FTextToSpeechSynthesizeAudioRequest>(Request);
	if (WatsonRequest != nullptr)
	{
		WatsonRequest->Progress->audioLength = BytesReceived;
	}
}

void UTextToSpeech::OnSynthesizeAudio(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FTextToSpeechSynthesizeAudioRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FTextToSpeechAudio> ResponseAudio = WatsonRequest->Progress;
		ResponseAudio->audioData = TArray<uint8>(Response->GetContent());	
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseAudio);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}