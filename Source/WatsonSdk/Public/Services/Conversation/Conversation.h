#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "Common/Authorization.h"
#include "Common/Configuration.h"
#include "MessageDataModel.h"
#include "Conversation.generated.h"

DECLARE_DELEGATE_TwoParams(FConversationMessageDelegate, TSharedPtr<FConversationMessageResponse>, TSharedPtr<FConversationMessageError>)


UCLASS()
class WATSONSDK_API UConversation : public UObject
{
	GENERATED_BODY()
public:
	FAuthorization Authorization;
	FConfiguration Configuration;
	UConversation();

	//////////////////////////////////////////////////////////////////////////
	// Send Message

private:
	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FConversationMessageDelegate>> PendingMessageRequests;
	void OnMessageComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
public:
	/**
	 * Get a response to a user's input.
	 * 
	 * @param Workspace		Unique identifier of the workspace. 
	 * @param Message		A MessageRequest object that provides the input text and optional context.
	 * @return				Returns the last user input, the recognized intents and entities, and the updated context and system output.
	 */
	TSharedPtr<FConversationMessageDelegate> Message(const FString& Workspace, const FConversationMessageRequest& Message);
};
