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

void UWatsonService::SetDefaults(FString Url, FString UserAgent, FString Version)
{
	ServiceUrl = Url;
	ServiceUserAgent = UserAgent;
	ServiceVersion = Version;
}

void UWatsonService::SendRequest(FWatsonRequest Request)
{
	Request.Send();
}

//////////////////////////////////////////////////////////////////////////
// Json Helpers

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
T UWatsonService::JsonObjectToStruct(const TSharedPtr<FJsonObject> JsonObject)
{
	TSharedPtr<T> Result = MakeShareable(new T);
	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), T::StaticStruct(), Result.Get(), 0, 0);
	return *Result;
}

template<typename T>
void UWatsonService::RemoveJsonArrayIfEmpty(FJsonObject* JsonObject, const FString& Field, const TArray<T>& Array)
{
	if (Array.Num() == 0 && JsonObject != nullptr)
	{
		JsonObject->RemoveField(Field);
	}
}


//////////////////////////////////////////////////////////////////////////
// Http Request Helpers

template<typename T>
T* UWatsonService::CreateWatsonRequest(TSharedPtr<IHttpRequest> Request)
{
	TSharedPtr<T> WatsonRequest = MakeShareable(new T);
	Requests.Add(Request, WatsonRequest);
	WatsonRequest->HttpRequest = Request;
	return WatsonRequest.Get();
}

template<typename T>
T* UWatsonService::RetrieveWatsonRequest(const FHttpRequestPtr& Request)
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

	T* CastWatsonRequest = (T*)WatsonRequest;
	if (CastWatsonRequest == nullptr)
	{
		return nullptr;
	}

	return CastWatsonRequest;
}

template<typename T>
bool UWatsonService::ValidateWatsonRequest(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, T*& OutWatsonRequest, FString& OutMessage)
{
	OutWatsonRequest = RetrieveWatsonRequest<T>(Request);
	if (OutWatsonRequest == nullptr)
	{
		OutMessage = "Error retrieving watson request.";
		return false;
	}
	if (!bWasSuccessful)
	{
		OutMessage = "Request was not successful.";
		return false;
	}
	if (Response->GetResponseCode() != 200)
	{
		OutMessage = "Request failed: " + Response->GetContentAsString();
		return false;
	}

	OutMessage = "Success!";
	return true;
}

/* Deprecated */
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