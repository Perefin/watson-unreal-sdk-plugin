#pragma once

#include "Configuration.generated.h"

USTRUCT()
struct FConfiguration
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString Url;
	UPROPERTY() FString UserAgent;
	UPROPERTY() FString Version;

	FConfiguration() {}
	FConfiguration(FString Url, FString UserAgent, FString Version) :
		Url(Url),
		UserAgent(UserAgent),
		Version(Version)
	{}
};
