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

FSpeechToTextRecognizeRequest USpeechToText::Recognize(TArray<uint8> AudioData, const FString& AudioModel, const FString& ContentType)
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
	return *CreateWatsonRequest<FSpeechToTextRecognizeRequest>(Request);
}


void USpeechToText::MakeSpeechToTextRequest(UObject* object,TArray<uint8> AudioData, FSpeechToTextRecognizeSuccess OnSuccess, FWatsonRequestFailure OnFailure)
{
	FSpeechToTextRecognizeRequest Request = Recognize(AudioData);
	Request.OnSuccess = OnSuccess;
	Request.OnFailure = OnFailure;
	Request.Send();
}

void USpeechToText::OnRecognize(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ErrorMessage;
	FSpeechToTextRecognizeRequest* WatsonRequest;
	if (ValidateWatsonRequest(Request, Response, bWasSuccessful, WatsonRequest, ErrorMessage))
	{
		TSharedPtr<FJsonObject> ResponseJson = StringToJsonObject(Response->GetContentAsString());
		FSpeechToTextRecognizeResponse ResponseStruct = JsonObjectToStruct<FSpeechToTextRecognizeResponse>(ResponseJson);
		WatsonRequest->OnSuccess.ExecuteIfBound(ResponseStruct);
		
	}
	else
	{
		WatsonRequest->OnFailure.ExecuteIfBound(ErrorMessage);
	}
	Requests.Remove(Request);
}