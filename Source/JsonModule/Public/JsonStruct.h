// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "JsonStruct.generated.h"

UENUM(BlueprintType)
enum class EJsonType : uint8 {
	None,
	Null,
	String,
	Number,
	Boolean,
	Array,
	Object
};


UCLASS(BlueprintType)
class UJsonStruct : public UObject
{
	GENERATED_BODY()
public:
	UJsonStruct()
	{
		value = nullptr;
		key = "";
	}
	virtual ~UJsonStruct()
	{

	}
public:
	TSharedPtr<FJsonValue> value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JsonStruct")
	FString key;
public:
	UFUNCTION(BlueprintPure, Category = "JsonStruct")
	FString GetStringValue();
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetStringValue(FString Value);
	UFUNCTION(BlueprintPure, Category = "JsonStruct")
		bool GetBoolValue();
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetBoolValue(bool Value);
	UFUNCTION(BlueprintPure, Category = "JsonStruct")
		float GetNumberValue();
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetNumberValue(float Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		UJsonStruct* GetArrayValue(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void AddArrayValueStruct(UJsonStruct* Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void AddArrayValueBool(bool Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void AddArrayValueNumber(float Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void AddArrayValueString(FString Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		UJsonStruct* GetObjectValue(FString ObjectKey);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetObjectValueStruct(UJsonStruct* Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetObjectValueBool(FString Jsonkey, bool Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetObjectValueNumber(FString Jsonkey, float Value);
	UFUNCTION(BlueprintCallable, Category = "JsonStruct")
		void SetObjectValueString(FString Jsonkey, FString Value);
};