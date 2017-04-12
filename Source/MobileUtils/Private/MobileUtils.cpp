// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2017 gameDNA. All Rights Reserved.

#include "IMobileUtils.h"
#include "MobileUtilsPrivatePCH.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "MobileUtilsPlatform.h"
#endif

DEFINE_LOG_CATEGORY(LogMobileUtils);

#define LOCTEXT_NAMESPACE "MobileUtils"

class FMobileUtils : public IMobileUtils
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FMobileUtils, MobileUtils)

// Startup Module
void FMobileUtils::StartupModule()
{	
	UE_LOG(LogMobileUtils,Warning,TEXT("FMobileUtils::StartupModule() start!"));
#if PLATFORM_ANDROID || PLATFORM_IOS
	PlatformInterface = MakeShareable(new FMobileUtilsPlatform());
#endif
}

// Shutdown Module
void FMobileUtils::ShutdownModule()
{
}


#undef LOCTEXT_NAMESPACE
