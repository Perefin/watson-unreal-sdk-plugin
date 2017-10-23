#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
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
	bool IsRequestSuccessful(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, FString& OutMessage);

	FString JsonObjectToString(const TSharedPtr<FJsonObject> JsonObject);

	TSharedPtr<FJsonObject> StringToJsonObject(const FString& String);

	template<typename T>
	FString StructToString(const T& Struct);

	template<typename T>
	TSharedPtr<T> StringToStruct(const FString& String);

	template<typename T>
	TSharedPtr<FJsonObject> StructToJsonObject(const T& Struct);

	template<typename T>
	TSharedPtr<T> JsonObjectToStruct(const TSharedPtr<FJsonObject> JsonObject);

	UPROPERTY()
	FAuthentication ServiceAuthentication;
	
	UPROPERTY()
	FString ServiceUrl;

	UPROPERTY()
	FString ServiceUserAgent;

	UPROPERTY()
	FString ServiceVersion;
};