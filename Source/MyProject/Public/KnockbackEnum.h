// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KnockbackEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EKnockBackEnum : uint8
{
	KE_NoKnockBack	UMETA(DisplayName = "NoKnockback"),
	KE_LightKnockBack	UMETA(DisplayName = "LightKnockback"),
	KE_HeavyKnockBack	UMETA(DisplayName = "HeavyKnockback")
};
