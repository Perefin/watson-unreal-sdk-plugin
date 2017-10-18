#pragma once

#include "Common/WatsonService.h"
#include "JsonUtilities.h"

TSharedRef<IHttpRequest> WatsonService::BuildRestRequest(
	const FString& Method,
	const FString& Url,
	const FString& Paths ...)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();	
	HttpRequest->SetVerb(Method);
	FString FullUrl = FString(Url);
	for (auto& Path : Paths)
	{
		FullUrl += "/";
		FullUrl += Path;
	}
	HttpRequest->SetURL(FullUrl);
	return HttpRequest;
}

void WatsonService::SetQueries(TSharedRef<IHttpRequest>& HttpRequest, const TArray<TTuple<FString, FString>>& Queries)
{
	FString FullUrl = FString(HttpRequest->GetURL());
	FullUrl += "?";
	for (auto& Query : Queries)
	{
		FullUrl += Query.Key + "=" + Query.Value;
	}
	HttpRequest->SetURL(FullUrl);
}

void WatsonService::SetHeaders(TSharedRef<IHttpRequest>& HttpRequest, const TArray<TTuple<FString, FString>>& Headers)
{
	for (auto& Header : Headers)
	{
		HttpRequest->SetHeader(Header.Key, Header.Value);
	}
}
