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
/*
FTextToSpeechSynthesizeAudioRequest* UTextToSpeech::SynthesizeAudio(const FString& Text, const FString& Voice, const FString& CustomizationId, const FString& Accept)
{
	TSharedPtr<FJsonObject> MessageJson = MakeShareable(new FJsonObject());
	MessageJson->SetStringField("text", Text);

	FString Path = ServiceUrl + "synthesize";
	Path += ("?voice=" + Voice);
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
*/

FSpeechToTextRecognizeRequest* USpeechToText::Recognize(TArray<uint8> AudioData, const FString& AudioModel, const FString& ContentType)
{
	FString Path = ServiceUrl + "recognize";
	Path += ("?model=" + AudioModel);
	
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(Path);
	Request->SetHeader(TEXT("User-Agent"), ServiceUserAgent);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Authorization"), ServiceAuthentication.Encode());
	Request->SetContent(AudioData);
	Request->OnProcessRequestComplete().BindUObject(this, &USpeechToText::OnRecognize);
	return CreateWatsonRequest<FSpeechToTextRecognizeRequest>(Request);
}

void USpeechToText::OnRecognize(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FSpeechToTextRecognizeRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		TSharedPtr<FSpeechToTextRecognizeResponse> ResponseStruct = JsonObjectToStruct<FSpeechToTextRecognizeResponse>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}