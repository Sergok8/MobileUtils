// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2017 gameDNA. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MobileUtilsBlueprintLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGPSLocation
{
	GENERATED_USTRUCT_BODY()
	double Latitude;
	double Longitude;
	double Altitude;
	UPROPERTY(BlueprintReadWrite, Category = "Mobile")
		float Accuracy;
};

USTRUCT(BlueprintType)
struct FGoogleSignInAccount
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Mobile")
		FString Id;
	UPROPERTY(BlueprintReadWrite, Category = "Mobile")
		FString Email;
	UPROPERTY(BlueprintReadWrite, Category = "Mobile")
		FString DisplayName;
};


UCLASS()
class MOBILEUTILS_API UMobileUtilsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckInternetConnection();

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckGooglePlayServices();

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
		static FString GetPersistentUniqueDeviceId();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils, meta=(Keywords="gps"))
		static bool GetCurrentLocation(FGPSLocation& Location);
	UFUNCTION(BlueprintPure, BlueprintPure, Category = MobileUtils, meta = (Keywords = "gps"))
		static void GetCoordinates(const FGPSLocation& Location, FString& Latitude, FString& Longitude, FString& Altitude);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta=(IntervalSeconds="5.0", Keywords="gps"))
		static void StartLocationUpdates(float IntervalSeconds);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "gps"))
		static void StopLocationUpdates();
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "login"))
		static void GetGoogleSignInAccount(FGoogleSignInAccount& Result, bool& bIsAvailable);
	UFUNCTION(BlueprintPure, Category = MobileUtils, meta = (Keywords = "gps"))
		static void ConvertGPS_ToWorld(const FGPSLocation& WorldGPS_Origin, const FGPSLocation& Location, FVector& WorldPosition);
	UFUNCTION(BlueprintPure, Category = MobileUtils, meta = (Keywords = "gps"))
		static void ConvertWorldToGPS(const FGPSLocation& WorldGPS_Origin, const FVector& WorldPosition, FGPSLocation& Location);
	UFUNCTION(BlueprintPure, Category = MobileUtils, meta = (Altitude = "0.0", Keywords = "gps"))
		static void SetCoordinates(const FString& Latitude, const FString& Longitude, const FString& Altitude, FGPSLocation& Location);

};
