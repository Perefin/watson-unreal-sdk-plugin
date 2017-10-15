#pragma once

#include "Runtime/Core/Public/Misc/Base64.h"
#include "Authorization.generated.h"

USTRUCT()
struct FAuthorization
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString username;
	
	UPROPERTY()
	FString password;
	
	FAuthorization() {}
	FAuthorization(FString username, FString password) :
		username(username),
		password(password)
	{}

	FString encode() {
		return "Basic " + FBase64::Encode(username + ":" + password);
	}
};
