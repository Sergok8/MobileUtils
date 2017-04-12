// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2017 gameDNA. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/MobileUtilsInterface.h"

typedef TSharedPtr<IMobileUtilsInterface, ESPMode::ThreadSafe> FMobileUtilsPlatformPtr;

class IMobileUtils : public IModuleInterface
{
public:
	static inline IMobileUtils& Get()
	{
		return FModuleManager::LoadModuleChecked< IMobileUtils >("MobileUtils");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("MobileUtils");
	}

	inline FMobileUtilsPlatformPtr GetPlatformInterface() const
	{
		return PlatformInterface;
	}

protected:
	FMobileUtilsPlatformPtr PlatformInterface;
};
