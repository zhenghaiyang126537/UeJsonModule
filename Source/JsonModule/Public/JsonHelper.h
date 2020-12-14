// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "JsonStruct.h"
#include "JsonHelper.generated.h"

UCLASS()
class JSONMODULE_API UJsonHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/*解析字符串*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static UJsonStruct* ParseJson(FString str);
	/*json生成字符串*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static FString ToString(UJsonStruct* json);
	/*获取类型*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static EJsonType JsonType(UJsonStruct* json);
	/*尝试获取Json基础类型value*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static FString TryGetBaseValue(UJsonStruct* json);
	/*获取JSON类型为Object和Array中的所有value*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static TArray<UJsonStruct*> GetAllValueFromJson(UJsonStruct* json);
	/*查找Json类型为Object中的特定Value*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Find"), Category = "Json")
		static UJsonStruct* FindJsonFromObject(UJsonStruct* json, FString key, EJsonType type,bool& isFound);
		static bool FindJsonFromObject_C(TSharedPtr<FJsonValue> json, FString key,EJson type, TSharedPtr<FJsonValue>& result);
	/*设置Json Key值*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static UJsonStruct* SetJsonKey(UJsonStruct* json,FString key);
	/*设置Json Value*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void ChangeJsonArrayValue(UJsonStruct* json, UJsonStruct* from, UJsonStruct* to, bool& isSuccess);
	UFUNCTION(BlueprintPure, Category = "Json")
		static void ChangeJsonObjectValue(UJsonStruct* json, FString key, UJsonStruct* value, bool& isSuccess);


	/*从Array中移除某一项*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void RemoveJsonArrayValueByValue(UJsonStruct* json, UJsonStruct* value,bool& isSuccess);
	/*从Objec中移除某一项*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void RemoveJsonObjectValueByKey(UJsonStruct* json, FString key, bool& isSuccess);
	/*增加Array一个元素*/
	UFUNCTION(BlueprintCallable, Category = "Json")
		static void AddJsonArrayValue(UJsonStruct* json, UJsonStruct* value, bool& isSuccess);
	/*增加Object一个元素*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void AddJsonObjectValue(UJsonStruct* json, UJsonStruct* value, bool& isSuccess,bool isRepleace = true);

	/*生成基础类型*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static UJsonStruct* CreateJsonStringValue(FString key, FString value);
	UFUNCTION(BlueprintPure, Category = "Json")
		static UJsonStruct* CreateJsonNumberValue(FString key, float value);
	UFUNCTION(BlueprintPure, Category = "Json")
		static UJsonStruct* CreateJsonBoolValue(FString key, bool value);
	/*生成空Object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateEmptyJsonObject"), Category = "Json")
		static UJsonStruct* CreateJsonObject();
	/*将多个Json合并到一个Object中 返回的Json类型为Object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateJsonObject"), Category = "Json")
		static UJsonStruct* CreateJsonObjectByValue(TArray<UJsonStruct*> arrays);
	static void CreateJsonObjectValue_C(TSharedPtr<FJsonObject> obj, UJsonStruct* addItem);

	/*生成空Array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateEmptyJsonArray"), Category = "Json")
		static UJsonStruct* CreateJsonArray();
	/*将多个Json合并到一个Aray中 返回的Json类型为Aray*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateJsonArray"), Category = "Json")
		static UJsonStruct* CreateJsonArrayByValue(TArray<UJsonStruct*> arrays);
	static void CreateJsonArrayValue_C(TArray<TSharedPtr<FJsonValue>>& ary, UJsonStruct* addItem);


	/*将json object转换为键值对*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Key-Value Pairs"), Category = "Json")
		static FString JsonObjectToKeyValuePairs(UJsonStruct* json);
};
