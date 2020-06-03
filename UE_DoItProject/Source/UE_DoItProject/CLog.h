#pragma once

#include "CoreMinimal.h"

class UE_DOITPROJECT_API CLog
{
public:
	CLog();
	~CLog();

	static void Log(int val);
	static void Log(float val);
	static void Log(const FString& val);
	static void Log(const TCHAR* val);
	static void Log(const FName& val);
	static void LogNull(UObject* object);

	static void Print(int val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(float val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FString& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const TCHAR* val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FName& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FVector& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void PrintNull(UObject* object, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
};
