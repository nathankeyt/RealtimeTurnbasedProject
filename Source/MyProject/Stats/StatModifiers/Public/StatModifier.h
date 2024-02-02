// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.generated.h"

class UStat;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UStatModifier : public UStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStat* ParentStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Modifier = 1.0f;

public:
	void SetParentStat(UStat* Stat) { ParentStat = Stat; }
	
	void SetModifier(const float Modifier_) { Modifier = Modifier_; }
};
