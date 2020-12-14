// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonHelper.h"
#include "StringTools.h"
UJsonStruct* UJsonHelper::ParseJson(FString str)
{
	TSharedPtr<FJsonValue> JsonParsed;
	MyTools::ParseJson(JsonParsed, str);
	UJsonStruct* out = NewObject<UJsonStruct>();
	out->value = JsonParsed;
	return out;
}

FString UJsonHelper::ToString(UJsonStruct* json)
{
	if(!json->value.IsValid())return FString();
	switch (json->value->Type) {
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:
		return TEXT("\"")+json->key+TEXT("\":")+TryGetBaseValue(json);
		break;
	case EJson::Array:
		return 	MyTools::CreateJson(json->value->AsArray());
		break;
	case EJson::Object:
		return 	MyTools::CreateJson(json->value->AsObject());
		break;
	}
	return 	FString();
}

EJsonType UJsonHelper::JsonType(UJsonStruct* json)
{
	switch (json->value->Type) {
	case EJson::None:return EJsonType::None;break;
	case EJson::Null:return EJsonType::Null;break;
	case EJson::String:return EJsonType::String;break;
	case EJson::Number:return EJsonType::Number;break;
	case EJson::Boolean:return EJsonType::Boolean;break;
	case EJson::Array:	return EJsonType::Array; break;
	case EJson::Object:	return EJsonType::Object; break;
	}
	return EJsonType::None;
}

FString UJsonHelper::TryGetBaseValue(UJsonStruct* json)
{
	return MyTools::ParseJsonValueSingle(json->value);
}

TArray<UJsonStruct*> UJsonHelper::GetAllValueFromJson(UJsonStruct* json)
{
	TArray<UJsonStruct*> out;
	if(!json->value.IsValid())	return out;
	if (json->value->Type == EJson::Object) {
		for (auto tem : json->value->AsObject()->Values) {
			UJsonStruct* addTem = NewObject<UJsonStruct>();
			addTem->value = tem.Value;
			addTem->key = tem.Key;
			out.Add(addTem);
		}
	}
	else if (json->value->Type == EJson::Array) {
		for (auto tem : json->value->AsArray()) {
			UJsonStruct* addTem = NewObject<UJsonStruct>();
			addTem->value = tem;
			out.Add(addTem);
		}
	}
	else out.Add(json);
	return out;
}

UJsonStruct* UJsonHelper::FindJsonFromObject(UJsonStruct* json, FString key,EJsonType type,bool& isFound)
{
	isFound = false;
	UJsonStruct* out = NewObject<UJsonStruct>();
	TSharedPtr<FJsonValue> result;
	if (FindJsonFromObject_C(json->value, key,(EJson)type,result)) {
		isFound = true;
		out->key = key;
		out->value = result;
	}
	return out;
}

bool UJsonHelper::FindJsonFromObject_C(TSharedPtr<FJsonValue> json, FString key,EJson type,TSharedPtr<FJsonValue>& result)
{
	if (!json.IsValid() || (json->Type != EJson::Object&&json->Type != EJson::Array))	return false;
	if (json->Type == EJson::Array) {
		auto temArray = json->AsArray();
		for (auto tem : temArray) {
			if (FindJsonFromObject_C(tem, key,type, result))return true;
		}
		return false;
	}
	else {
		switch (type)
		{
		case EJson::None:case EJson::Null:return false; break;
		case EJson::String: {
			auto tem = json->AsObject()->GetField<EJson::String>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Number: {
			auto tem = json->AsObject()->GetField<EJson::String>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Boolean: {
			auto tem = json->AsObject()->GetField<EJson::String>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Array: {
			auto tem = json->AsObject()->GetField<EJson::Array>(key);
			if (tem.IsValid()){
				result = tem;
				return true;
			}
			break;
		}
		case EJson::Object: {
			auto tem = json->AsObject()->GetField<EJson::Object>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			break;
		}
		}
		return false;
	}
}

UJsonStruct* UJsonHelper::SetJsonKey(UJsonStruct* json, FString key)
{
	json->key = key;
	return json;
}

void UJsonHelper::ChangeJsonArrayValue(UJsonStruct* json, UJsonStruct* from, UJsonStruct* to, bool& isSuccess)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Array)return ;
	if (!from->value.IsValid() || !to->value.IsValid())return ;
	auto arrays = json->value->AsArray();
	auto index = 0;
	for (auto tem : arrays) {
		UJsonStruct* temStruct = NewObject<UJsonStruct>();
		temStruct->key = from->key;
		temStruct->value = tem;
		if (ToString(temStruct).Equals(ToString(from))) {
			arrays[index] = to->value;
			TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(arrays));
			json->value = Value;
			isSuccess = true;
			return ;
		}
		index++;
	}
}

void UJsonHelper::ChangeJsonObjectValue(UJsonStruct* json, FString key, UJsonStruct* value, bool& isSuccess)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Object)return ;
	isSuccess = json->value->AsObject()->HasField(key);
	json->value->AsObject()->SetField(key, value->value);
}


void UJsonHelper::RemoveJsonArrayValueByValue(UJsonStruct* json, UJsonStruct* value, bool& isSuccess)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Array)return ;
	if (!value->value.IsValid())return ;
	auto arrays = json->value->AsArray();
	auto index = 0;
	for (auto tem : arrays) {
		UJsonStruct* temStruct = NewObject<UJsonStruct>();
		temStruct->key = value->key;
		temStruct->value = tem;
		if (ToString(temStruct).Equals(ToString(value))) {
			arrays.RemoveAt(index);
			TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(arrays));
			json->value = Value;
			isSuccess = true;
			return ;
		}
		index++;
	}
	return ;
}

void UJsonHelper::RemoveJsonObjectValueByKey(UJsonStruct* json, FString key, bool& isSuccess)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Object)return ;
	isSuccess=json->value->AsObject()->HasField(key);
	if(isSuccess)
		json->value->AsObject()->RemoveField(key);
	//return json;
}

void UJsonHelper::AddJsonArrayValue(UJsonStruct*  json, UJsonStruct* value, bool & isSuccess)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Array)return;
	if (!value->value.IsValid())return;
	auto temArray = json->value->AsArray();
	temArray.Add(value->value);
	TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(temArray));
	json->value = Value;
	isSuccess = true;
}
void UJsonHelper::AddJsonObjectValue(UJsonStruct* json, UJsonStruct* value, bool& isSuccess, bool isRepleace)
{
	isSuccess = false;
	if (!json->value.IsValid() || json->value->Type != EJson::Object)return ;
	if (!value->value.IsValid())return ;
	if (isRepleace) {
		json->value->AsObject()->SetField(value->key, value->value);
		isSuccess = true;
		return ;
	}
	else {
		if (json->value->AsObject()->HasField(value->key))return ;
		json->value->AsObject()->SetField(value->key, value->value);
		isSuccess = true;
		return ;
	}
}

UJsonStruct* UJsonHelper::CreateJsonStringValue(FString key, FString value)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	TSharedPtr<FJsonValueString> Value = MakeShareable(new FJsonValueString(value));
	out->key = key;
	out->value = Value;
	return out;
}

UJsonStruct* UJsonHelper::CreateJsonNumberValue(FString key, float value)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	TSharedPtr<FJsonValueNumber> Value = MakeShareable(new FJsonValueNumber(value));
	out->key = key;
	out->value = Value;
	return out;
}

UJsonStruct* UJsonHelper::CreateJsonBoolValue(FString key, bool value)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	TSharedPtr<FJsonValueBoolean> Value = MakeShareable(new FJsonValueBoolean(value));
	out->key = key;
	out->value = Value;
	return out;
}
UJsonStruct* UJsonHelper::CreateJsonObject()
{
	TArray<UJsonStruct*> tem;
	return CreateJsonObjectByValue(tem);
}
UJsonStruct* UJsonHelper::CreateJsonObjectByValue(TArray<UJsonStruct*> arrays)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	TSharedPtr<FJsonObject> temObj = MakeShareable(new FJsonObject);
	TSharedPtr < FJsonValueObject > Value = MakeShareable(new FJsonValueObject(temObj));
	for (auto tem : arrays) {
		if (tem->value.IsValid())CreateJsonObjectValue_C(temObj, tem);
	}
	out->value = Value;
	return out;
}


void UJsonHelper::CreateJsonObjectValue_C(TSharedPtr<FJsonObject> obj, UJsonStruct* addItem)
{
	switch (addItem->value->Type)
	{
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:case EJson::Array:case EJson::Object:
		obj->SetField(addItem->key, addItem->value);
		break;
	}
}

UJsonStruct* UJsonHelper::CreateJsonArray()
{
	TArray<UJsonStruct*> tem;
	return CreateJsonArrayByValue(tem);
}

UJsonStruct* UJsonHelper::CreateJsonArrayByValue(TArray<UJsonStruct*> arrays)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	TArray<TSharedPtr<FJsonValue>> temArray;
	for (auto tem : arrays)
		if (tem->value.IsValid())CreateJsonArrayValue_C(temArray, tem);
	TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(temArray));
	out->value = Value;
	return out;
}

void UJsonHelper::CreateJsonArrayValue_C(TArray<TSharedPtr<FJsonValue>>& ary, UJsonStruct* addItem)
{
	switch (addItem->value->Type)
	{
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:case EJson::Object:case EJson::Array:
		ary.Add(addItem->value);
		break;
	}
}

FString UJsonHelper::JsonObjectToKeyValuePairs(UJsonStruct* json)
{
	FString out;
	if (!json->value.IsValid() || json->value->Type != EJson::Object)return out;
	auto object=json->value->AsObject();
	for (auto tem : object->Values) {
		out+=tem.Key+ TEXT("=")+MyTools::ParseJsonValueSingle(tem.Value)+TEXT("&");
	}
	out.RemoveFromEnd(TEXT("&"));
	return out;
}
