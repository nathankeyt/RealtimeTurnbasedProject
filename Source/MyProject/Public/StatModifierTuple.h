// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.h"
#include "Engine/DataAsset.h"
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
	EStatEnum StatEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UStatModifier> StatModifierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ModifierValue;

public:
	EStatEnum GetStatEnum() const
	{
		return StatEnum;
	}

	TSubclassOf<UStatModifier> GetStatModifierClass() const
	{
		return StatModifierClass;
	}

	float GetModifierValue() const
	{
		return ModifierValue;
	}
};
