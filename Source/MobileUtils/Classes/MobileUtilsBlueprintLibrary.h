// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

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
	UPROPERTY(BlueprintReadWrite)
		float Accuracy;
};

USTRUCT(BlueprintType)
struct FGoogleSignInAccount
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite)
		FString Id;
	UPROPERTY(BlueprintReadWrite)
		FString Email;
	UPROPERTY(BlueprintReadWrite)
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils, meta = (Keywords = "gps"))
		static void GetCoordinates(const FGPSLocation& Location, FString& Latitude, FString& Longitude, FString& Altitude);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta=(IntervalSeconds="5.0", Keywords="gps"))
		static void StartLocationUpdates(float IntervalSeconds);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "gps"))
		static void StopLocationUpdates();
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "login"))
		static void GetGoogleSignInAccount(FGoogleSignInAccount& Result, bool& bIsAvailable);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "gps"))
		void ConvertGPSToWorld(const FGPSLocation& WorldGPSOrigin, const FGPSLocation& Location, FVector& WorldPosition);
	UFUNCTION(BlueprintCallable, Category = MobileUtils, meta = (Keywords = "gps"))
		void ConvertWorldToGPS(const FGPSLocation& WorldGPSOrigin, const FVector& WorldPosition, FGPSLocation& Location);
};
