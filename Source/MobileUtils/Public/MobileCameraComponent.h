// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Components/ActorComponent.h"
#include "MobileCameraComponent.generated.h"

UCLASS( ClassGroup=(Mobile), meta=(BlueprintSpawnableComponent) )
class MOBILEUTILS_API UMobileCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMobileCameraComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Mobile", meta = (Keywords = "camera"))
	UTexture* VideoTexture;
	
	UPROPERTY(BlueprintReadOnly, Category = "Mobile", meta = (Keywords = "camera"))
	FVector2D VideoSize;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game starts
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	bool bCameraIsInitialized;
};
