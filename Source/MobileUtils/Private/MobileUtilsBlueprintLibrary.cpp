// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#include "MobileUtilsPrivatePCH.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


UMobileUtilsBlueprintLibrary::UMobileUtilsBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UMobileUtilsBlueprintLibrary::CheckInternetConnection()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->CheckInternetConnection();
#else
	return true;
#endif
}

bool UMobileUtilsBlueprintLibrary::CheckGooglePlayServices()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->CheckGooglePlayServices();
#else
	return false;
#endif
}

FString UMobileUtilsBlueprintLibrary::GetPersistentUniqueDeviceId()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->GetPersistentUniqueDeviceId();
#else
	return UKismetSystemLibrary::GetUniqueDeviceId();
#endif
}

bool UMobileUtilsBlueprintLibrary::GetCurrentLocation(FGPSLocation& Location)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	double Coords[4];
	bool Result = IMobileUtils::Get().GetPlatformInterface()->GetCurrentLocation(Coords);
	if (Result) {
		Location.Latitude = Coords[0];
		Location.Longitude = Coords[1];
		Location.Altitude = Coords[2];
		Location.Accuracy = (float)Coords[3];
	}
	return Result;
#else
	return false;
#endif
}

void UMobileUtilsBlueprintLibrary::StartLocationUpdates(float IntervalSeconds)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->StartLocationUpdates(IntervalSeconds);
#endif
}

void UMobileUtilsBlueprintLibrary::StopLocationUpdates()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->StopLocationUpdates();
#endif
}

void UMobileUtilsBlueprintLibrary::GetCoordinates(const FGPSLocation& Location,
	FString& Latitude,
	FString& Longitude,
	FString& Altitude)
{
	Latitude = FString::Printf(TEXT("%.17f"), Location.Latitude);
	Longitude = FString::Printf(TEXT("%.17f"), Location.Longitude);
	Altitude = FString::Printf(TEXT("%.17f"), Location.Altitude);
}

void UMobileUtilsBlueprintLibrary::GetGoogleSignInAccount(FGoogleSignInAccount& Result, bool& bIsAvailable)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	bIsAvailable = IMobileUtils::Get().GetPlatformInterface()->GetGoogleSignInAccount(&Result);
#else
	bIsAvailable = false;
#endif
}

// from http://danceswithcode.net/engineeringnotes/geodetic_to_ecef/geodetic_to_ecef.html

const double  a = 6378137.0;              //WGS-84 semi-major axis
const double e2 = 6.6943799901377997e-3;  //WGS-84 first eccentricity squared
const double a1 = 4.2697672707157535e+4;  //a1 = a*e2
const double a2 = 1.8230912546075455e+9;  //a2 = a1*a1
const double a3 = 1.4291722289812413e+2;  //a3 = a1*e2/2
const double a4 = 4.5577281365188637e+9;  //a4 = 2.5*a2
const double a5 = 4.2840589930055659e+4;  //a5 = a1+a3
const double a6 = 9.9330562000986220e-1;  //a6 = 1-e2


//Convert Earth-Centered-Earth-Fixed (ECEF) to lat, Lon, Altitude
//Input is a three element array containing x, y, z in meters
//Returned array contains lat and lon in radians, and altitude in meters
static void ecef_to_geo(const double* ecef, double* geo) //Results go in geo (Lat, Lon, Altitude)
{
	double zp, w2, w, r2, r, s2, c2, s, c, ss;
	double g, rg, rf, u, v, m, f, p, x, y, z;
	  
	x = ecef[0];
	y = ecef[1];
	z = ecef[2];
	zp = FMath::Abs(z);
	w2 = x*x + y*y;
	w = FMath::Sqrt(w2);
	r2 = w2 + z*z;
	r = FMath::Sqrt(r2);
	geo[1] = FMath::Atan2(y, x);       //Lon (final)
	s2 = z*z / r2;
	c2 = w2 / r2;
	u = a2 / r;
	v = a3 - a4 / r;
	if (c2 > 0.3) {
		s = (zp / r)*(1.0 + c2*(a1 + u + s2*v) / r);
		geo[0] = FMath::Asin(s);      //Lat
		ss = s*s;
		c = FMath::Sqrt(1.0 - ss);
	}
	else {
		c = (w / r)*(1.0 - s2*(a5 - u - c2*v) / r);
		geo[0] = FMath::Acos(c);      //Lat
		ss = 1.0 - c*c;
		s = FMath::Sqrt(ss);
	}
	g = 1.0 - e2*ss;
	rg = a / FMath::Sqrt(g);
	rf = a6*rg;
	u = w - rg*c;
	v = zp - rf*s;
	f = c*u + s*v;
	m = c*v - s*u;
	p = m / (rf / g + f);
	geo[0] = geo[0] + p;      //Lat
	geo[2] = f + m*p / 2.0;     //Altitude
	if (z < 0.0) {
		geo[0] *= -1.0;     //Lat
	}
}

//Convert Lat, Lon, Altitude to Earth-Centered-Earth-Fixed (ECEF)
//Input is a three element array containing lat, lon (rads) and alt (m)
//Returned array contains x, y, z in meters
static void geo_to_ecef(const double* geo, double* ecef)//Results go in ecef (x, y, z)
{
	double n, lat, lon, alt;
	lat = geo[0];
	lon = geo[1];
	alt = geo[2];
	n = a / FMath::Sqrt(1 - e2*FMath::Sin(lat)*FMath::Sin(lat));
	ecef[0] = (n + alt)*FMath::Cos(lat)*FMath::Cos(lon);    //ECEF x
	ecef[1] = (n + alt)*FMath::Cos(lat)*FMath::Sin(lon);    //ECEF y
	ecef[2] = (n*(1 - e2) + alt)*FMath::Sin(lat);          //ECEF z
}


void UMobileUtilsBlueprintLibrary::ConvertGPS_ToWorld(const FGPSLocation& WorldOrigin, const FGPSLocation& Location, FVector& WorldPosition)
{
	double P0[3];
	double P1[3];
	geo_to_ecef(&WorldOrigin.Latitude, P0);
	geo_to_ecef(&Location.Latitude, P1);
	float Forward = float(P1[1] - P0[1]);
	float Right = float(P1[0] - P0[0]);
	float Up = float(P1[2] - P0[2]);
	WorldPosition = FVector(Forward * 100, Right * 100, Up * 100);
}

void UMobileUtilsBlueprintLibrary::ConvertWorldToGPS(const FGPSLocation& WorldOrigin, const FVector& WorldPosition, FGPSLocation& Location)
{
	double P0[3];
	geo_to_ecef(&WorldOrigin.Latitude, P0);
	P0[0] += WorldPosition.Y / 100.0;
	P0[1] += WorldPosition.X / 100.0;
	P0[2] += WorldPosition.Z / 100.0;
	ecef_to_geo(P0, &Location.Latitude);
}

void UMobileUtilsBlueprintLibrary::SetCoordinates(const FString& Latitude, const FString& Longitude, const FString& Altitude,
	FGPSLocation& Location)
{
	Location.Latitude = FCString::Atod(*Latitude);
	Location.Longitude = FCString::Atod(*Longitude);
	Location.Altitude = FCString::Atod(*Altitude);
}