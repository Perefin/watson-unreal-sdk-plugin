#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "WatsonService.h"
#include "MessageDataModel.h"
#include "Conversation.generated.h"

UCLASS()
class WATSONSDK_API UConversation : public UWatsonService
{
	GENERATED_BODY()

public:
	UConversation();



//////////////////////////////////////////////////////////////////////////
// Send Message

private:
	TMap<TSharedPtr<IHttpRequest>, TSharedPtr<FConversationMessagePendingRequest>> PendingMessageRequests;
	void OnMessageComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
public:
	/**
	 * Get a response to a user's input.
	 * 
	 * @param Workspace		Unique identifier of the workspace. 
	 * @param Message		A MessageRequest object that provides the input text and optional context.
	 * @return				Returns the last user input, the recognized intents and entities, and the updated context and system output.
	 */
	FConversationMessagePendingRequest* Message(const FString& Workspace, const FConversationMessageRequest& Message);
};
