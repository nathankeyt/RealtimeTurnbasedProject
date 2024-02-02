// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackLevelEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackLevelEnum : uint8
{
	AE_LightAttack	UMETA(DisplayName = "LightAttack"),
	AE_HeavyAttack	UMETA(DisplayName = "HeavyAttack")
};
