# watson-sdk-plugin

## Watson Conversation
```cpp
UConversation* MyConversation = CreateDefaultSubobject<UConversation>(TEXT("Conversation"));
MyConversation->Authorization.username = /* username */;
MyConversation->Authorization.password = /* password */;

FConversationMessageRequest ConversationRequest;
ConversationRequest.input.text = "Hello there, how are you?";
MyConversation->Message(/* workspace */, ConversationRequest)->BindUObject(this, &AMyActor::OnConversationMessage);

void AMyActor::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response, TSharedPtr<FConversationMessageError> Error)
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->output.text[0]);
}
```

## Watson Speech To Text
```cpp
USpeechToText* MySpeechToText = CreateDefaultSubobject<USpeechToText>(TEXT("SpeechToText"));
MySpeechToText->Authorization.username = /* username */;
MySpeechToText->Authorization.password = /* password */;

MySpeechToText->Recognize(MySpeaker->GetRecording(), "en-US_BroadbandModel")->BindUObject(this, &AMyActor::OnSpeechToTextRecognize);

void AMyActor::OnSpeechToTextRecognize(TSharedPtr<FSpeechRecognitionEvent> Response, TSharedPtr<FSpeechToTextError> Error)
{
  for (auto& result : Response->results)
    for (auto& alternative : result.alternatives)
      UE_LOG(LogTemp, Warning, TEXT("%s"), *alternative.transcript);
}
```

## Watson Text To Speech
```cpp
UTextToSpeech* MyTextToSpeech = CreateDefaultSubobject<UTextToSpeech>(TEXT("TextToSpeech"));
MyTextToSpeech->Authorization.username = /* username */;
MyTextToSpeech->Authorization.password = /* password */;

FSynthesizeRequest SynthesisRequest;
SynthesisRequest.text = "Hello there, how are you?";
MyTextToSpeech->Synthesize(SynthesisRequest)->BindUObject(this, &AMyActor::OnTextToSpeechSynthesize);

void AMyActor::OnTextToSpeechSynthesize(TSharedPtr<FSynthesizeResponse> Response, TSharedPtr<FSynthesizeError> Error)
{
  MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}
```

## Speaker
```cpp
USpeaker* MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));

MySpeaker->PlayAudio(/* raw audio */, /* raw audio length */);
```

## Microphone
```cpp
UMicrophone* MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

MyMicrophone->StartRecording();
MyMicrophone->StopRecording();
MyMicrophone->GetRecording();
```
