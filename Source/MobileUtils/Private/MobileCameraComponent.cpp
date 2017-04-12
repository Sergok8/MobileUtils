// Fill out your copyright notice in the Description page of Project Settings.

#include "MobileCameraComponent.h"
#include "IMobileUtils.h"
#include "MobileUtilsPrivatePCH.h"

// Sets default values for this component's properties
UMobileCameraComponent::UMobileCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	VideoTexture = nullptr;
	bCameraIsInitialized = false;
	// ...
}

// Called when the game starts
void UMobileCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	if (VideoTexture == nullptr)
	{
//		VideoTexture = UTexture2D::CreateTransient(1, 1, PF_R8G8B8A8);
//		VideoTexture->UpdateResource();
		
		//	VideoTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("Texture2D'/Game/Mannequin/Character/Textures/UE4_LOGO_CARD.UE4_LOGO_CARD'")));
		VideoTexture = UTexture2D::CreateTransient(1, 1, PF_R8G8B8A8);
		if (!VideoTexture)
		{
			VLOG(LogMobileUtils, "Cannot create video texture!");
		}
		else
		{
			VideoTexture->UpdateResource();
			VLOGM(LogMobileUtils, "Created video texture, dimensions: %f x %f", VideoTexture->GetSurfaceWidth(), VideoTexture->GetSurfaceHeight());
		}

		
	}
 
}

void UMobileCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#if PLATFORM_ANDROID
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(StopTexture,
		bool*, CamInitPtr, &bCameraIsInitialized,
		{
			if (*CamInitPtr)
			{
				*CamInitPtr = false;
				IMobileUtils::Get().GetPlatformInterface()->StopCamera();
			}
		});
#endif
}


// Called every frame
void UMobileCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

#if PLATFORM_ANDROID
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(UpdateTexture,
		FTextureRHIRef, Tex, VideoTexture->Resource->TextureRHI,
		FVector2D*, DimPtr, &VideoSize,
		bool*, bCamInitPtr, &bCameraIsInitialized,
		{
			void* Res = Tex->GetNativeResource();
			if (Res != nullptr)
			{
				int32 TextureId = *reinterpret_cast<int32*>(Res);
				if (!*bCamInitPtr)
				{
					*bCamInitPtr = true;
					IMobileUtils::Get().GetPlatformInterface()->StartCamera();
					int32 Width = IMobileUtils::Get().GetPlatformInterface()->GetCameraWidth();
					int32 Height = IMobileUtils::Get().GetPlatformInterface()->GetCameraHeight();
					*DimPtr = FVector2D(Width, Height);
				}
				IMobileUtils::Get().GetPlatformInterface()->UpdateCameraTexture(TextureId);
			}
		});
	
#endif
}

