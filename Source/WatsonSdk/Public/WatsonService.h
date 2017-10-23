#pragma once

#include "CoreMinimal.h"
#include "Common/Authentication.h"
#include "WatsonService.generated.h"

UCLASS()
class WATSONSDK_API UWatsonService : public UObject
{
GENERATED_BODY()

public:
	UWatsonService();
	void SetAuthentication(FAuthentication Authentication);
	void SetUrl(FString Url);
	void SetUserAgent(FString UserAgent);
	void SetVersion(FString Version);

protected:
	UPROPERTY()
	FAuthentication ServiceAuthentication;
	
	UPROPERTY()
	FString ServiceUrl;

	UPROPERTY()
	FString ServiceUserAgent;

	UPROPERTY()
	FString ServiceVersion;
};