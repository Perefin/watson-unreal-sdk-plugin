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