// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2017 gameDNA. All Rights Reserved.

#pragma once

class IMobileUtilsInterface
{
public:
	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	virtual bool CheckInternetConnection() = 0;

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	virtual bool CheckGooglePlayServices() = 0;

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	virtual FString GetPersistentUniqueDeviceId() = 0;
	
	/**
	 * Selects a photo and returns a corresponding texture
	 * @CameraOrFile: 0 - Camera, 1 - File
	 */
	virtual void SelectPhotoTexture(int32 CameraOrFile) = 0;


	virtual bool GetCurrentLocation(double* Coords) = 0;
	virtual void StartLocationUpdates(float IntervalSeconds) = 0;
	virtual void StopLocationUpdates() = 0;
	virtual bool GetGoogleSignInAccount(struct FGoogleSignInAccount* Result) = 0;

	virtual void StartCamera() = 0;
	virtual int32 GetCameraWidth() = 0;
	virtual int32 GetCameraHeight() = 0;
	virtual bool UpdateCameraTexture(int32 DestTextureId) = 0;
	virtual void StopCamera() = 0;
};
