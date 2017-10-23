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
