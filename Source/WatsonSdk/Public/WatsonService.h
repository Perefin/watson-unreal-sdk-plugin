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
	// Json Helpers

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
	
	template<typename T>
	void RemoveJsonArrayIfEmpty(FJsonObject* JsonObject, const FString& Field, const TArray<T>& Array);


	//////////////////////////////////////////////////////////////////////////
	// Http Request Helpers

	template<typename T>
	T* CreateWatsonRequest(TSharedPtr<IHttpRequest> Request);

	template<typename T>
	bool ValidateWatsonRequest(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, T*& OutWatsonRequest, FString& OutMessage);


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