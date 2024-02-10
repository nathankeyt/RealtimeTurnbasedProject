// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyProject/Stats/Public/Stat.h"
#include "StatModifierTuple.generated.h"

enum class EStatEnum : uint8;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UStatModifierTuple : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UStatModifier> StatModifierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStat* StatModifier;

public:

	TSubclassOf<UStatModifier> GetStatModifierClass() const
	{
		return StatModifierClass;
	}

	UStat* GetStatModifier() const
	{
		return StatModifier;
	}

	float GetStatModifierValue() const
	{
		if (StatModifier != nullptr)
		{
			return StatModifier->GetData();
		}
		return 1.0f;
	}

	void SetStatModifier(UStat* NewStatModifier)
	{
		StatModifier = NewStatModifier;
	}
};
