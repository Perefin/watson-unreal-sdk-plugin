#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
#include "Common/Authentication.h"
#include "WatsonService.generated.h"

DECLARE_DELEGATE_OneParam(FWatsonRequestFailure, FString)

USTRUCT()
struct FWatsonRequest
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<IHttpRequest> HttpRequest;
	FWatsonRequestFailure OnFailure;

	void Send()
	{
		HttpRequest->ProcessRequest();
	}
};

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
	//////////////////////////////////////////////////////////////////////////
	// Json Serializers & Deserializers

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


	//////////////////////////////////////////////////////////////////////////
	// Http Request Helpers

	TSharedPtr<IHttpRequest> CreateRequest(FString Verb, FString Path);

	void AddHeader(IHttpRequest* Request, FString HeaderName, FString HeaderValue, bool AddIfEmptyString = false);

	void AddQuery(IHttpRequest* Request, FString QueryName, FString QueryValue, bool AddIfEmptyString = false);

	template<typename T>
	T* NewWatsonRequest(TSharedPtr<IHttpRequest> Request);

	template<typename T>
	T* GetWatsonRequest(TSharedPtr<IHttpRequest> Request);

	bool IsRequestSuccessful(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, FString& OutMessage);


	//////////////////////////////////////////////////////////////////////////
	// Properties

	UPROPERTY()
	FAuthentication ServiceAuthentication;

	UPROPERTY()
	FString ServiceUrl;

	UPROPERTY()
	FString ServiceUserAgent;

	UPROPERTY()
	FString ServiceVersion;

	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FWatsonRequest>> Requests;
};