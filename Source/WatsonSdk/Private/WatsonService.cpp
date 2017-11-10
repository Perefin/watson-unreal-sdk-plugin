#pragma once

#include "WatsonService.h"

UWatsonService::UWatsonService() {}

void UWatsonService::SetAuthentication(FAuthentication Authentication)
{
	ServiceAuthentication = Authentication;
}

void UWatsonService::SetUrl(FString Url)
{
	ServiceUrl = Url;
}

void UWatsonService::SetUserAgent(FString UserAgent)
{
	ServiceUserAgent = UserAgent;
}

void UWatsonService::SetVersion(FString Version)
{
	ServiceVersion = Version;
}


//////////////////////////////////////////////////////////////////////////
// Json Serializers & Deserializers

FString UWatsonService::JsonObjectToString(const TSharedPtr<FJsonObject> JsonObject)
{
	FString Result;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Result);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return Result;
}

TSharedPtr<FJsonObject> UWatsonService::StringToJsonObject(const FString& String)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
	FJsonSerializer::Deserialize(Reader, JsonObject);
	return JsonObject;
}

template<typename T>
FString UWatsonService::StructToString(const T& Struct)
{
	FString Result;
	FJsonObjectConverter::UStructToJsonObjectString(T::StaticStruct(), &Struct, Result, 0, 0);
	return Result;
}

template<typename T>
TSharedPtr<T> UWatsonService::StringToStruct(const FString& String)
{
	TSharedPtr<T> Result = MakeShareable(new T);
	FJsonObjectConverter::JsonObjectStringToUStruct<T>(String, Result.Get(), 0, 0);
	return Result;
}

template<typename T>
TSharedPtr<FJsonObject> UWatsonService::StructToJsonObject(const T& Struct)
{
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &Struct, Result.ToSharedRef(), 0, 0);
	return Result;
}

template<typename T>
TSharedPtr<T> UWatsonService::JsonObjectToStruct(const TSharedPtr<FJsonObject> JsonObject)
{
	TSharedPtr<T> Result = MakeShareable(new T);
	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), T::StaticStruct(), Result.Get(), 0, 0);
	return Result;
}


//////////////////////////////////////////////////////////////////////////
// Http Request Helpers

TSharedPtr<IHttpRequest> UWatsonService::CreateRequest(FString Verb, FString Path)
{
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(Verb);
	Request->SetURL(Path);
	return Request;
}

void UWatsonService::AddHeader(IHttpRequest* Request, FString HeaderName, FString HeaderValue, bool AddIfEmptyString)
{
	if (AddIfEmptyString || !HeaderValue.IsEmpty())
	{
		Request->SetHeader(HeaderName, HeaderValue);
	}
}

void UWatsonService::AddQuery(IHttpRequest* Request, FString QueryName, FString QueryValue, bool AddIfEmptyString)
{
	if (AddIfEmptyString || !QueryValue.IsEmpty())
	{
		FString Original = Request->GetURL();
		Request->SetURL(Original + QueryName + "=" + QueryValue + "&");
	}
}

bool UWatsonService::IsRequestSuccessful(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, class FString& OutMessage)
{
	if (!bWasSuccessful)
	{
		OutMessage = "Request was not successful.";
		return false;
	}
	else if (Response->GetResponseCode() != 200)
	{
		OutMessage = "Request failed: " + Response->GetContentAsString();
		return false;
	}
	else
	{
		return true;
	}
}

template<typename T>
T* UWatsonService::NewWatsonRequest(TSharedPtr<IHttpRequest> Request)
{
	TSharedPtr<T> WatsonRequest = MakeShareable(new T);
	Requests.Add(Request, WatsonRequest);
	WatsonRequest->HttpRequest = Request;
	return WatsonRequest.Get();
}

template<typename T>
T* UWatsonService::GetWatsonRequest(TSharedPtr<IHttpRequest> Request)
{
	TSharedPtr<FWatsonRequest>* WatsonRequestPtr = Requests.Find(Request);
	if (WatsonRequestPtr == nullptr)
	{
		return nullptr;
	}

	FWatsonRequest* WatsonRequest = WatsonRequestPtr->Get();
	if (WatsonRequest == nullptr)
	{
		return nullptr;
	}

	T* CastWatsonRequest = (T*) WatsonRequest;
	if (CastWatsonRequest == nullptr)
	{
		return nullptr;
	}

	return CastWatsonRequest;
}