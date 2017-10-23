#pragma once

#include "Runtime/Core/Public/Misc/Base64.h"
#include "Authentication.generated.h"

USTRUCT()
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

	UPROPERTY()
	FString Username;

	UPROPERTY()
	FString Password;
};
