// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#pragma once

#include "Interfaces/MobileUtilsInterface.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include <android_native_app_glue.h>

class FMobileUtilsPlatform : public IMobileUtilsInterface
{
public:
	FMobileUtilsPlatform();
	virtual ~FMobileUtilsPlatform();


	virtual bool CheckInternetConnection() override;
	virtual bool CheckGooglePlayServices() override;
	virtual FString GetPersistentUniqueDeviceId() override;
	virtual bool GetCurrentLocation(double* Coordinates);
	virtual void StartLocationUpdates(float IntervalSeconds);
	virtual void StopLocationUpdates();
	virtual bool GetGoogleSignInAccount(struct FGoogleSignInAccount* Result) override;

	// JNI Methods
	static jmethodID CheckInternetConnectionMethod;
	static jmethodID CheckGooglePlayServicesMethod;
	static jmethodID GetPersistentUniqueDeviceIdMethod;
	static jmethodID GetCurrentLocationMethod;
	static jmethodID StartLocationUpdatesMethod;
	static jmethodID StopLocationUpdatesMethod;
	static jmethodID GetGoogleSignInAccountMethod;
	static jmethodID GoogleSignInAccountIdMethod;
	static jmethodID GoogleSignInAccountEmailMethod;
	static jmethodID GoogleSignInAccountDisplayNameMethod;
};
