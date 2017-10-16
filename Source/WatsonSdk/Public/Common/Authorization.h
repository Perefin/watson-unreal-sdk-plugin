#pragma once

#include "Runtime/Core/Public/Misc/Base64.h"
#include "Authorization.generated.h"

USTRUCT()
struct FAuthorization
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Username;
	
	UPROPERTY()
	FString Password;
	
	FAuthorization() {}
	FAuthorization(FString username, FString password) :
		Username(username),
		Password(password)
	{}

	FString Encode() {
		return "Basic " + FBase64::Encode(Username + ":" + Password);
	}
};
