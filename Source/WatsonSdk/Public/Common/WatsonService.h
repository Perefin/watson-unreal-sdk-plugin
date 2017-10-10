#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "Common/Authorization.h"
#include "Common/Configuration.h"

class WATSONSDK_API WatsonService
{
protected:
	FAuthorization Authorization;
	FConfiguration Configuration;

	TSharedRef<IHttpRequest> BuildRestRequest(const FString& Method, const FString& Url, const FString& Paths ...);
	void SetQueries(TSharedRef<IHttpRequest>& Request, const TArray<TTuple<FString, FString>>& Queries);
	void SetHeaders(TSharedRef<IHttpRequest>& Request, const TArray<TTuple<FString, FString>>& Headers);
};