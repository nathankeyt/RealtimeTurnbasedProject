 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.h"
#include "StatMultiplierModifier.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UStatMultiplierModifier : public UStatModifier
{
	GENERATED_BODY()

public:
	float GetData() const override { return ParentStat->GetData() * Modifier; };
};
