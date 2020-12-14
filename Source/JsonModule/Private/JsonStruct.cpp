// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonStruct.h"

FString UJsonStruct::GetStringValue()
{
	if (value && value->Type == EJson::String)
	{
		return value->AsString();
	}
	return "";
}
void UJsonStruct::SetStringValue(FString Value)
{
	value = MakeShareable(new FJsonValueString(Value));
}
bool UJsonStruct::GetBoolValue()
{
	if (value && value->Type == EJson::Boolean)
	{
		return value->AsBool();
	}
	return false;
}
void UJsonStruct::SetBoolValue(bool Value)
{
	value = MakeShareable(new FJsonValueBoolean(Value));
}
float UJsonStruct::GetNumberValue()
{
	if (value && value->Type == EJson::Number)
	{
		return value->AsNumber();
	}
	return 0;
}
void UJsonStruct::SetNumberValue(float Value)
{
	value = MakeShareable(new FJsonValueNumber(Value));
}
UJsonStruct* UJsonStruct::GetArrayValue(int32 Index)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	if (value && value->Type == EJson::Array)
	{
		out->value = value->AsArray()[Index];
		return out;
	}
	return out;
}
void UJsonStruct::AddArrayValue(UJsonStruct* Value)
{
	if (value == nullptr)
	{
		TArray<TSharedPtr<FJsonValue>> EmptyArray;
		value = MakeShareable(new FJsonValueArray(EmptyArray));
	}
	TArray<TSharedPtr<FJsonValue>> temArray= value->AsArray();
	temArray.Add(Value->value);
	value = MakeShareable(new FJsonValueArray(temArray));
}

UJsonStruct* UJsonStruct::GetObjectValue(FString ObjectKey)
{
	UJsonStruct* out = NewObject<UJsonStruct>();
	if (value && value->Type == EJson::Object)
	{
		out->value = value->AsObject()->GetField<EJson::None>(ObjectKey);
		return out;
	}
	return out;
}
void UJsonStruct::SetObjectValue(FString ObjectKey, UJsonStruct* Value)
{
	if (nullptr == value)
	{
		value = MakeShareable(new FJsonValueObject(MakeShareable(new FJsonObject)));
	}
	value->AsObject()->SetField(ObjectKey, Value->value);
}