// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "SpeakerComponent.generated.h"


UCLASS(ClassGroup = (WatsonSDK), meta = (BlueprintSpawnableComponent), BlueprintType)
class WATSONSDK_API USpeakerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpeakerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void PlayAudio(const TArray<uint8>& AudioData, uint32 AudioLength);

private:
	UPROPERTY()
		UAudioComponent* AudioOutputComponent;

	UPROPERTY(Transient, Instanced)
		USoundWaveProcedural* AudioPCMComponent;
	
};
