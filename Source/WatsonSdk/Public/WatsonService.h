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

UCLASS(Blueprintable, BlueprintType, Category = "Watson")
class WATSONSDK_API UWatsonService : public UObject
{
	GENERATED_BODY()

public:
	UWatsonService();
	UFUNCTION(BlueprintCallable)
	void SetAuthentication(FAuthentication Authentication);
	UFUNCTION(BlueprintCallable)
	void SetUrl(FString Url);
	UFUNCTION(BlueprintCallable)
	void SetUserAgent(FString UserAgent);
	UFUNCTION(BlueprintCallable)
	void SetVersion(FString Version);
	UFUNCTION(BlueprintCallable)
	virtual void SetDefaults(FString Url, FString UserAgent, FString Version);

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
	T* RetrieveWatsonRequest(const FHttpRequestPtr& Request);

	template<typename T>
	bool ValidateWatsonRequest(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, T*& OutWatsonRequest, FString& OutMessage);

	/* Deprecated */
    bool IsRequestSuccessful(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bWasSuccessful, FString& OutMessage);


	//////////////////////////////////////////////////////////////////////////
	// Properties

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAuthentication ServiceAuthentication;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServiceUrl;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ServiceUserAgent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServiceVersion;

	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FWatsonRequest>> Requests;
};