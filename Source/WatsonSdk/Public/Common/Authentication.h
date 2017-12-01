#pragma once

#include "Runtime/Core/Public/Misc/Base64.h"
#include "Authentication.generated.h"

USTRUCT(BlueprintType)
struct FAuthentication
{
	GENERATED_USTRUCT_BODY()
	
	FAuthentication() {}
	
	FAuthentication(const FString Username, const FString Password)
		: Username(Username), Password(Password)
	{}

	FString Encode()
	{
		return "Basic " + FBase64::Encode(Username + ":" + Password);
	}

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString Username;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString Password;
};
