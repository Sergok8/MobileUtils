// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#include "MobileUtilsPrivatePCH.h"
#include "MobileUtilsPlatform.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "MobileutilsBlueprintLibrary.h"


jmethodID FMobileUtilsPlatform::CheckInternetConnectionMethod;
jmethodID FMobileUtilsPlatform::CheckGooglePlayServicesMethod;
jmethodID FMobileUtilsPlatform::GetPersistentUniqueDeviceIdMethod;
jmethodID FMobileUtilsPlatform::GetCurrentLocationMethod;
jmethodID FMobileUtilsPlatform::StartLocationUpdatesMethod;
jmethodID FMobileUtilsPlatform::StopLocationUpdatesMethod;
jmethodID FMobileUtilsPlatform::GetGoogleSignInAccountMethod;
jmethodID FMobileUtilsPlatform::GoogleSignInAccountIdMethod;
jmethodID FMobileUtilsPlatform::GoogleSignInAccountEmailMethod;
jmethodID FMobileUtilsPlatform::GoogleSignInAccountDisplayNameMethod;
static jmethodID GetGoogleSignInAccountClassMethod;
static jclass GoogleSignInAccountClass;

jmethodID FMobileUtilsPlatform::StartCameraMethod;
jmethodID FMobileUtilsPlatform::GetCameraWidthMethod;
jmethodID FMobileUtilsPlatform::GetCameraHeightMethod;
jmethodID FMobileUtilsPlatform::UpdateCameraTextureMethod;
jmethodID FMobileUtilsPlatform::StopCameraMethod;

FMobileUtilsPlatform::FMobileUtilsPlatform()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
	    CheckInternetConnectionMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckInternetConnection", "()Z", false);
		CheckGooglePlayServicesMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckGooglePlayServices", "()Z", false);
		GetPersistentUniqueDeviceIdMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPersistentUniqueDeviceId", "()Ljava/lang/String;", false);
		GetCurrentLocationMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetCurrentLocation", "([D)Z", false);
		StartLocationUpdatesMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StartLocationUpdates", "(I)V", false);
		StopLocationUpdatesMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StopLocationUpdates", "()V", false);
		GetGoogleSignInAccountMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetGoogleSignInAccount", "()Ljava/lang/Object;", false);
		GetGoogleSignInAccountClassMethod = FJavaWrapper::FindStaticMethod(Env, FJavaWrapper::GameActivityClassID, "GetGoogleSignInAccountClass", "()Ljava/lang/Class;", false);
		
		StartCameraMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StartCamera", "()V", false);
		StopCameraMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StopCamera", "()V", false);
		GetCameraWidthMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetCameraWidth", "()I", false);
		GetCameraHeightMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetCameraHeight", "()I", false);
		UpdateCameraTextureMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_UpdateCameraTexture", "(I)Z", false);
	}
}

FMobileUtilsPlatform::~FMobileUtilsPlatform()
{
}

void FMobileUtilsPlatform::StartLocationUpdates(float IntervalSeconds)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) 
	{
		jint value = jint(FMath::RoundToInt(IntervalSeconds * 1000));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::StartLocationUpdatesMethod, value);
	}
}

bool FMobileUtilsPlatform::GetGoogleSignInAccount(struct FGoogleSignInAccount* Result)
{
	if (Result == nullptr)
	{
		// error
		return false;
	}
	if (GoogleSignInAccountClass == 0)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jclass Clazz = (jclass)Env->CallStaticObjectMethod(FJavaWrapper::GameActivityClassID, GetGoogleSignInAccountClassMethod);
			GoogleSignInAccountIdMethod = FJavaWrapper::FindMethod(Env, Clazz, "getId", "()Ljava/lang/String;", false);
			GoogleSignInAccountDisplayNameMethod = FJavaWrapper::FindMethod(Env, Clazz, "getDisplayName", "()Ljava/lang/String;", false);
			GoogleSignInAccountEmailMethod = FJavaWrapper::FindMethod(Env, Clazz, "getEmail", "()Ljava/lang/String;", false);
			GoogleSignInAccountClass = Clazz;
		}
	}
	bool bIsAvailable = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jobject result = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetGoogleSignInAccountMethod);
		bIsAvailable = result != NULL;
		if (false && bIsAvailable)
		{
			{
				jstring jstr = (jstring) FJavaWrapper::CallObjectMethod(Env, result, FMobileUtilsPlatform::GoogleSignInAccountIdMethod);
				const char *str = Env->GetStringUTFChars(jstr, 0);
				Result->Id = FString(str);
				Env->ReleaseStringUTFChars(jstr, str);
				Env->DeleteLocalRef(jstr);
			}
			{
				jstring jstr = (jstring) FJavaWrapper::CallObjectMethod(Env, result, FMobileUtilsPlatform::GoogleSignInAccountDisplayNameMethod);
				const char *str = Env->GetStringUTFChars(jstr, 0);
				Result->DisplayName = FString(str);
				Env->ReleaseStringUTFChars(jstr, str);
				Env->DeleteLocalRef(jstr);	
			}
			{
				jstring jstr = (jstring) FJavaWrapper::CallObjectMethod(Env, result, FMobileUtilsPlatform::GoogleSignInAccountEmailMethod);
				const char *str = Env->GetStringUTFChars(jstr, 0);
				Result->Email = FString(str);
				Env->ReleaseStringUTFChars(jstr, str);
				Env->DeleteLocalRef(jstr);
			}
			Env->DeleteLocalRef(result);
		}
		else if (bIsAvailable) // google account id only
		{
			jstring jstr = (jstring)result;
			const char *str = Env->GetStringUTFChars(jstr, 0);
			Result->Id = FString(str);
			Env->ReleaseStringUTFChars(jstr, str);
			Env->DeleteLocalRef(jstr);
			Result->DisplayName = Result->Id;
			Result->Email = Result->Id;
		}

	}
	return bIsAvailable;
}

void FMobileUtilsPlatform::StopLocationUpdates()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::StopLocationUpdatesMethod);
	}
}

bool FMobileUtilsPlatform::GetCurrentLocation(double* Coordinates)
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jdoubleArray Array = Env->NewDoubleArray(4);
		if (Array == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("JNI NewDoubleArray failed"));
		}
		else
		{
			bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetCurrentLocationMethod, Array);
		}
		if (bResult)
		{
			Env->GetDoubleArrayRegion(Array, 0, 4, Coordinates);
		}
		else
		{
			for (int32 i = 0; i < 4; i++) Coordinates[i] = 0;
		}
		if (Array != nullptr)
		{
			Env->DeleteLocalRef(Array);
		}
		Env->ExceptionClear();

	}
	return bResult;
}

bool FMobileUtilsPlatform::CheckInternetConnection()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::CheckInternetConnectionMethod);
	}
	return bResult;
}

bool FMobileUtilsPlatform::CheckGooglePlayServices()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::CheckGooglePlayServicesMethod);
	}
	return bResult;
}

FString FMobileUtilsPlatform::GetPersistentUniqueDeviceId()
{
	FString ResultDeviceId = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetPersistentUniqueDeviceIdMethod);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		ResultDeviceId = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return ResultDeviceId;
}


bool FMobileUtilsPlatform::UpdateCameraTexture(int32 DestTexture)
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::UpdateCameraTextureMethod, DestTexture);
	}
	return bResult;
}

void FMobileUtilsPlatform::StartCamera()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::StartCameraMethod);
	}
}

void FMobileUtilsPlatform::StopCamera()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::StopCameraMethod);
	}
}

int32 FMobileUtilsPlatform::GetCameraWidth()
{
	int32 Result = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		Result = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetCameraWidthMethod);
	}
	return Result;
}

int32 FMobileUtilsPlatform::GetCameraHeight()
{
	int32 Result = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		Result = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetCameraHeightMethod);
	}
	return Result;
}